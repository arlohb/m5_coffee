#include "LvglState.h"

LvglState::LvglState(const char* titleText) {
    root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES - StatusBar::HEIGHT);
    lv_obj_align(root, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(root, LV_BORDER_SIDE_NONE, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(root, 16, LV_STATE_DEFAULT);

    lv_obj_t* title = lv_label_create(root);
    lv_label_set_text(title, titleText);
    lv_obj_set_style_text_font(title, &lv_font_fira_code_20, LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);
}

LvglState::~LvglState() {
    lv_obj_delete(root);
}

void LvglState::loop() {
    statusBar.loop();
}
