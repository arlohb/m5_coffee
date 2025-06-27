#include "ScalesState.h"

#include "../Utils.h"

ScalesState::ScalesState() : LvglState("Scales") {
    xTaskCreate([] (void* arg) {
        ScalesState* state = static_cast<ScalesState*>(arg);
        Scales* scales = &state->scales;

        while (true) {
            state->weight = scales->getWeight();
            delay(200);
        }
    }, "Scales Loop", 4 * 1024, this, 1, &loopTask);
    
    lv_obj_t* zero = lv_btn_create(root);
    lv_obj_set_size(zero, 100, 50);
    lv_obj_align(zero, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_event_cb(zero, [] (lv_event_t* e) {
        ScalesState* state = static_cast<ScalesState*>(lv_event_get_user_data(e));
        state->scales.setOffset();
    }, LV_EVENT_PRESSED, this);
    
    lv_obj_t* zeroLabel = lv_label_create(zero);
    lv_label_set_text(zeroLabel, "Zero");
    lv_obj_center(zeroLabel);
    
    weightLabel = lv_label_create(root);
    lv_label_set_text(weightLabel, "00.0g");
    lv_obj_center(weightLabel);
    lv_obj_set_style_text_font(weightLabel, &lv_font_fira_code_32, 0);
}

ScalesState::~ScalesState() {
    vTaskDelete(loopTask);
}

State* ScalesState::loop() {
    State* newState = LvglState::loop();
    if (newState) return newState;
    
    lv_label_set_text(weightLabel, fmt::format("{:04.1f}g", weight.load()).c_str());

    return nullptr;
}
