#include "MenuState.h"

#include "HistoryState.h"
#include "ScalesState.h"
#include "../Utils.h"

MenuState::MenuState() : LvglState("m5_coffee", false) {
    lv_obj_t* btnRow = lv_obj_create(root);
    lv_obj_remove_style_all(btnRow);
    lv_obj_set_style_pad_all(btnRow, LvglState::PADDING, 0);
    lv_obj_set_size(btnRow, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_layout(btnRow, LV_LAYOUT_FLEX);
    lv_obj_set_style_pad_column(btnRow, 8, 0);
    lv_obj_remove_flag(btnRow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* v60Btn = lv_btn_create(btnRow);
    lv_obj_add_event_cb(v60Btn, [](lv_event_t* e) {
        LOG_INFO("V60 button clicked");
    }, LV_EVENT_CLICKED, nullptr);
    lv_obj_set_flex_grow(v60Btn, 1);

    lv_obj_t* v60Label = lv_label_create(v60Btn);
    lv_label_set_text(v60Label, "V60");
    lv_obj_center(v60Label);

    lv_obj_t* espressoBtn = lv_btn_create(btnRow);
    lv_obj_add_event_cb(espressoBtn, [](lv_event_t* e) {
        MenuState* state = static_cast<MenuState*>(lv_event_get_user_data(e));
        
        state->stateTransition = []() {
            return new HistoryState();
        };
    }, LV_EVENT_CLICKED, this);
    lv_obj_set_flex_grow(espressoBtn, 1);

    lv_obj_t* espressoLabel = lv_label_create(espressoBtn);
    lv_label_set_text(espressoLabel, "Esp");
    lv_obj_center(espressoLabel);

    lv_obj_t* scalesBtn = lv_btn_create(btnRow);
    lv_obj_add_event_cb(scalesBtn, [](lv_event_t* e) {
        MenuState* state = static_cast<MenuState*>(lv_event_get_user_data(e));

        state->stateTransition = []() {
            return new ScalesState([](float _weight) {
                return []() { return new MenuState(); };
            });
        };
    }, LV_EVENT_CLICKED, this);
    lv_obj_set_flex_grow(scalesBtn, 1);

    lv_obj_t* scalesLabel = lv_label_create(scalesBtn);
    lv_label_set_text(scalesLabel, "Scales");
    lv_obj_center(scalesLabel);
}

std::optional<StateTransition> MenuState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    return std::nullopt;
}
