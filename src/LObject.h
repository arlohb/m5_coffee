#pragma once

#include <functional>
#include <lvgl.h>

class LObject {
public:
    LObject() = default;
    LObject(lv_obj_t *ptr);

    LObject(const LObject&) = delete;
    LObject& operator=(const LObject&) = delete;

    LObject(LObject &&);
    LObject& operator=(LObject&&);

    void SetPos(uint16_t x, uint16_t y);
    void SetSize(uint16_t x, uint16_t y);
    void Align(lv_align_t alignment, int32_t x, int32_t y);

    template<typename T>
    void AddCallback(void(*callback)(T*), T* data, lv_event_code_t event);
    template<typename T>
    void OnPress(void(*callback)(T*), T* data);

public:
    lv_obj_t* ptr;
};

template<typename T>
void LObject::AddCallback(void(*callback)(T*), T* userData, lv_event_code_t event) {
    struct Data {
        T* data;
        void(*callback)(T*);
    };
    
    Data* data = new Data();
    data->data = userData;
    data->callback = callback;

    lv_event_cb_t cb = [](lv_event_t* e) {
        auto dataPtr = static_cast<Data*>(lv_event_get_user_data(e));
        dataPtr->callback(dataPtr->data);
    };
    lv_obj_add_event_cb(ptr, cb, event, data);
}

template<typename T>
void LObject::OnPress(void(*callback)(T*), T* data) {
    AddCallback<T>(callback, data, LV_EVENT_PRESSED);
}
