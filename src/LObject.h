#pragma once

#include <functional>
#include <lvgl.h>

using Callback = std::function<void()>;

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
    void Align(lv_align_t alignment, uint16_t x, uint16_t y);

    void AddCallback(Callback callback, lv_event_code_t event);
    void OnPress(Callback callback);

public:
    lv_obj_t* ptr;
};

