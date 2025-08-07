#include "InScalesState.h"

#undef B1
#include <fmt/core.h>
#include "BrewState.h"

InScalesState::InScalesState(std::function<StateTransition(float)> next):
    LvglState("In Scales", true),
    nextStateTransition(next)
{
    lv_obj_t* weightBtn = lv_btn_create(root);
    lv_obj_set_size(weightBtn, 128, 64);
    lv_obj_align(weightBtn, LV_ALIGN_CENTER, 0, 16);
    lv_obj_add_event_cb(weightBtn, [] (lv_event_t* e) {
        InScalesState* state = static_cast<InScalesState*>(lv_event_get_user_data(e));
        state->scales.zero();
    }, LV_EVENT_PRESSED, this);

    weightLabel = lv_label_create(weightBtn);
    lv_label_set_text(weightLabel, "00.0g");
    lv_obj_center(weightLabel);
    lv_obj_set_style_text_font(weightLabel, &lv_font_fira_code_32, 0);
    
    lv_obj_t* nextBtn = lv_btn_create(root);
    lv_obj_set_size(nextBtn, 80, 40);
    lv_obj_align(nextBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(nextBtn, [](lv_event_t* e) {
        InScalesState* state = static_cast<InScalesState*>(lv_event_get_user_data(e));
        
        float weight = state->scales.getWeight();
        state->stateTransition = state->nextStateTransition(weight);
    }, LV_EVENT_PRESSED, this);
    
    lv_obj_t* nextLabel = lv_label_create(nextBtn);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);
}

std::optional<StateTransition> InScalesState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    lv_label_set_text(weightLabel, fmt::format("{:04.1f}g", scales.getWeight()).c_str());

    return std::nullopt;
}
