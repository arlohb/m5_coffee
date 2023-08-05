#pragma once

#include <functional>
#include <lvgl.h>

using Callback = std::function<void()>;

class LObject {
    public:
        lv_obj_t* ptr;

        LObject() = default;
        LObject(lv_obj_t* ptr);

        void SetPos(uint16_t x, uint16_t y);
        void SetSize(uint16_t x, uint16_t y);
        void Align(lv_align_t alignment, uint16_t x, uint16_t y);

        void AddCallback(Callback callback, lv_event_code_t event);
        void OnPress(Callback callback);
};

