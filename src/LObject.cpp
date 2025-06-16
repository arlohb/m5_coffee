#include "LObject.h"

LObject::LObject(lv_obj_t* ptr):
    ptr(ptr)
{}

LObject::LObject(LObject&& other) {
    ptr = other.ptr;
    other.ptr = 0;
}

LObject& LObject::operator=(LObject&& other) {
    ptr = other.ptr;
    other.ptr = 0;
    return *this;
}

void LObject::SetPos(uint16_t x, uint16_t y) {
    lv_obj_set_pos(ptr, x, y);
}

void LObject::SetSize(uint16_t x, uint16_t y) {
    lv_obj_set_size(ptr, x, y);
}

void LObject::Align(lv_align_t alignment, uint16_t x, uint16_t y) {
    lv_obj_align(ptr, alignment, x, y);
}

void LObject::AddCallback(Callback callback, lv_event_code_t event) {
    static Callback staticCallback = callback;

    lv_obj_add_event_cb(ptr, [](lv_event_t* e) {
        staticCallback();
    }, event, NULL);
}

void LObject::OnPress(Callback callback) {
    AddCallback(callback, LV_EVENT_PRESSED);
}

