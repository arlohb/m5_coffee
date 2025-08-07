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
    
    auto createBtn = [this, btnRow](const char* label, lv_event_cb_t eventCb) {
        lv_obj_t* btn = lv_btn_create(btnRow);
        lv_obj_add_event_cb(btn, eventCb, LV_EVENT_CLICKED, this);
        lv_obj_set_flex_grow(btn, 1);

        lv_obj_t* labelObj = lv_label_create(btn);
        lv_label_set_text(labelObj, label);
        lv_obj_center(labelObj);

        return btn;
    };
    
    createBtn("V60", [](lv_event_t* e) {
        LOG_INFO("V60 button clicked");
    });
    
    createBtn("Esp", [](lv_event_t* e) {
        MenuState* state = static_cast<MenuState*>(lv_event_get_user_data(e));
        
        state->stateTransition = []() {
            return new HistoryState();
        };
    });
    
    createBtn("Scales", [](lv_event_t* e) {
        MenuState* state = static_cast<MenuState*>(lv_event_get_user_data(e));
        
        state->stateTransition = []() {
            return new ScalesState([](float _weight) {
                return []() { return new MenuState(); };
            });
        };
    });
}

std::optional<StateTransition> MenuState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    return std::nullopt;
}
