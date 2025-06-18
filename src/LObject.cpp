#include "LObject.h"

#include <lvgl.h>

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

void LObject::Align(lv_align_t alignment, int32_t x, int32_t y) {
    lv_obj_align(ptr, alignment, x, y);
}
