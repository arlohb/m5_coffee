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

void LObject::setPos(uint16_t x, uint16_t y) {
    lv_obj_set_pos(ptr, x, y);
}

void LObject::setSize(uint16_t x, uint16_t y) {
    lv_obj_set_size(ptr, x, y);
}

void LObject::align(lv_align_t alignment, int32_t x, int32_t y) {
    lv_obj_align(ptr, alignment, x, y);
}
