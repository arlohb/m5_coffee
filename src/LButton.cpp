#include "LButton.h"

LButton::LButton(LObject& parent) {
    ptr = lv_btn_create(parent.ptr);
}

void LButton::setLabel(const std::string& text) {
    if (!label) {
        label = LLabel(*this);
        label->align(LV_ALIGN_CENTER, 0, 0);
    }

    label->setText(text);
}
