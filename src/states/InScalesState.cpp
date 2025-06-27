#include "InScalesState.h"

#undef B1
#include <fmt/core.h>
#include "BrewState.h"

InScalesState::InScalesState(const std::string& selectedCoffee) :
    LvglState("In Scales", true),
    selectedCoffee(selectedCoffee)
{
    weightLabel = lv_label_create(root);
    lv_label_set_text(weightLabel, "00.0g");
    lv_obj_align(weightLabel, LV_ALIGN_CENTER, -50, 0);
    lv_obj_set_style_text_font(weightLabel, &lv_font_fira_code_32, 0);
    
    lv_obj_t* zero = lv_btn_create(root);
    lv_obj_set_size(zero, 80, 40);
    lv_obj_align(zero, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(zero, [] (lv_event_t* e) {
        InScalesState* state = static_cast<InScalesState*>(lv_event_get_user_data(e));
        state->scales.zero();
    }, LV_EVENT_PRESSED, this);

    lv_obj_t* zeroLabel = lv_label_create(zero);
    lv_label_set_text(zeroLabel, "Zero");
    lv_obj_center(zeroLabel);
    
    lv_obj_t* nextBtn = lv_btn_create(root);
    lv_obj_set_size(nextBtn, 80, 40);
    lv_obj_align(nextBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(nextBtn, [](lv_event_t* e) {
        InScalesState* state = static_cast<InScalesState*>(lv_event_get_user_data(e));
        state->stateTransition = []() {
            return new BrewState();
        };
    }, LV_EVENT_PRESSED, this);
    
    lv_obj_t* nextLabel = lv_label_create(nextBtn);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);
}

StateTransition InScalesState::loop() {
    StateTransition stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    lv_label_set_text(weightLabel, fmt::format("{:04.1f}g", scales.getWeight()).c_str());

    return std::nullopt;
}
