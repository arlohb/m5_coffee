#include "LvglState.h"

#include "MainState.h"
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
    
    stateSwitcher = lv_obj_create(lv_scr_act());
    lv_obj_align(stateSwitcher, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_layout(stateSwitcher, LV_LAYOUT_FLEX, LV_STATE_DEFAULT);
    lv_obj_set_style_flex_flow(stateSwitcher, LV_FLEX_FLOW_ROW, LV_STATE_DEFAULT);
    lv_obj_set_size(stateSwitcher, lv_pct(100), 32);
    lv_obj_set_style_pad_all(stateSwitcher, 2, LV_STATE_DEFAULT);
    
    lv_obj_t* mainBtn = lv_btn_create(stateSwitcher);
    lv_obj_add_event_cb(mainBtn, [] (lv_event_t* e) {
        State** newState = (State**)lv_event_get_user_data(e);
        *newState = new MainState();
    }, LV_EVENT_PRESSED, &newState);
    
    lv_obj_t* historyBtn = lv_btn_create(stateSwitcher);
    lv_obj_add_event_cb(historyBtn, [] (lv_event_t* e) {
        State** newState = (State**)lv_event_get_user_data(e);
        *newState = new HistoryState();
    }, LV_EVENT_PRESSED, &newState);
    
    lv_obj_t* scalesBtn = lv_btn_create(stateSwitcher);
    lv_obj_add_event_cb(scalesBtn, [] (lv_event_t* e) {
        State** newState = (State**)lv_event_get_user_data(e);
        *newState = new ScalesState();
    }, LV_EVENT_PRESSED, &newState);
    
    std::pair<lv_obj_t*, const char*> btns[] = {
        {mainBtn, "Main"},
        {historyBtn, "History"},
        {scalesBtn, "Scales"}
    };

    for (auto& [btn, labelText] : btns) {
        lv_obj_set_style_pad_all(btn, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, lv_theme_get_color_secondary(root), LV_STATE_DEFAULT);

        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, labelText);
        lv_obj_center(label);
        lv_obj_set_style_pad_all(label, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_flex_grow(btn, 1, LV_STATE_DEFAULT);
    }
}

LvglState::~LvglState() {
    lv_obj_delete(root);
    lv_obj_delete(stateSwitcher);
}

State* LvglState::loop() {
    if (newState) return newState;

    statusBar.loop();
    
    return nullptr;
}
