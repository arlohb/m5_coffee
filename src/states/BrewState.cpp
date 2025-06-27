#include "BrewState.h"

#undef B1
#include <fmt/core.h>
#include <M5Unified.h>

BrewState::BrewState(
    const std::string& selectedCoffee,
    float inWeight,
    float ratio
) :
    LvglState("Brew"),
    selectedCoffee(selectedCoffee),
    inWeight(inWeight),
    ratio(ratio),
    targetWeight(inWeight * ratio)
{
    timer.registerOnPress();
    lv_obj_set_parent(timer.btn, root);
    lv_obj_align(timer.btn, LV_ALIGN_TOP_RIGHT, 0, 0);
    
    lv_obj_t* weightBtn = lv_btn_create(root);
    lv_obj_set_size(weightBtn, 128, 64);
    lv_obj_align(weightBtn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_add_event_cb(weightBtn, [] (lv_event_t* e) {
        BrewState* state = static_cast<BrewState*>(lv_event_get_user_data(e));
        state->scales.zero();
    }, LV_EVENT_PRESSED, this);
    
    weightLabel = lv_label_create(weightBtn);
    lv_label_set_text(weightLabel, "00.0g");
    lv_obj_center(weightLabel);
    lv_obj_set_style_text_font(weightLabel, &lv_font_fira_code_32, 0);
    
    lv_obj_t* targetWeightLabel = lv_label_create(root);
    lv_label_set_text(targetWeightLabel, fmt::format("{:.1f}g", targetWeight).c_str());
    lv_obj_align(targetWeightLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);
}

StateTransition BrewState::loop() {
    StateTransition stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    timer.update();
    
    lv_label_set_text(weightLabel, fmt::format("{:04.1f}g", scales.getWeight()).c_str());
    
    return std::nullopt;
}
