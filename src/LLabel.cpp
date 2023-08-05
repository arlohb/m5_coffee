#include "LLabel.h"

LLabel::LLabel(LObject& parent) {
    ptr = lv_label_create(parent.ptr);
}

void LLabel::SetText(std::string text) {
    lv_label_set_text(ptr, text.c_str());
}

