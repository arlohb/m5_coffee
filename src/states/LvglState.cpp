#include "LvglState.h"

#include "BrewState.h"
#include "HistoryState.h"
#include "ScalesState.h"
#include <utility>

LvglState::LvglState(const char* titleText, bool padding) {
    root = lv_obj_create(lv_scr_act());
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES - StatusBar::HEIGHT);
    lv_obj_align(root, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(root, LV_BORDER_SIDE_NONE, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(root, padding ? 16 : 0, LV_STATE_DEFAULT);

    lv_obj_t* title = lv_label_create(root);
    lv_label_set_text(title, titleText);
    // If main widget isn't padded, pad manually
    if (padding) {
        lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);
    } else {
        lv_obj_align(title, LV_ALIGN_TOP_LEFT, 16, 16);
    }
}

LvglState::~LvglState() {
    lv_obj_delete(root);
}

StateTransition LvglState::loop() {
    {
        std::lock_guard<std::mutex> lock(stateTransitionMutex);
        if (stateTransition) return stateTransition;
    }

    statusBar.loop();
    
    return std::nullopt;
}
