#include "LButton.h"

LButton::LButton(LObject& parent) {
    ptr = lv_btn_create(parent.ptr);
}

void LButton::SetLabel(const std::string& text) {
    if (!label) {
        label = LLabel(*this);
        label->Align(LV_ALIGN_CENTER, 0, 0);
    }

    label->SetText(text);
}
