#include "BrewState.h"

#undef B1
#include <fmt/core.h>
#include <M5Unified.h>
#include "../CoffeeDB.h"
#include "MenuState.h"

float roundToDp(float value, int dp) {
    float factor = std::pow(10.0f, dp);
    return std::round(value * factor) / factor;
}

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
    
    lv_obj_t* finishBtn = lv_btn_create(root);
    lv_obj_set_size(finishBtn, 80, 40);
    lv_obj_align(finishBtn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(finishBtn, [](lv_event_t* e) {
        BrewState* state = static_cast<BrewState*>(lv_event_get_user_data(e));
        
        xTaskCreate([](void* arg) {
            BrewState* state = static_cast<BrewState*>(arg);

            auto now = std::chrono::system_clock::now();
            unsigned long timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

            CoffeeDB::add({
                .timestamp = timestamp,
                .coffee = state->selectedCoffee,
                .in = roundToDp(state->inWeight, 1),
                .ratio = state->ratio,
                .aimOut = roundToDp(state->targetWeight, 2),
                .actualOut = roundToDp(state->scales.getWeight(), 1),
                .brewTime = roundToDp(state->timer.getElapsedSeconds(), 1),
            });

            {
                std::lock_guard<std::mutex> lock(state->stateTransitionMutex);
                state->stateTransition = []() {
                    return new MenuState();
                };
            }
            
            vTaskDelete(nullptr);
        }, "save_brew", 8 * 1024, state, 1, nullptr);
    }, LV_EVENT_PRESSED, this);
    
    lv_obj_t* finishLabel = lv_label_create(finishBtn);
    lv_label_set_text(finishLabel, "Finish");
    lv_obj_center(finishLabel);
}

std::optional<StateTransition> BrewState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    timer.update();
    
    lv_label_set_text(weightLabel, fmt::format("{:04.1f}g", scales.getWeight()).c_str());
    
    return std::nullopt;
}
