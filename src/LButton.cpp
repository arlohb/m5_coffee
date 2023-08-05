#include "LButton.h"

LButton::LButton(LObject& parent) {
    ptr = lv_btn_create(parent.ptr);
}

