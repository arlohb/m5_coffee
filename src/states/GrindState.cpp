#include "GrindState.h"

#include "BrewState.h"
#include "../Utils.h"

GrindState::GrindState(const std::string& selectedCoffee, float weight) :
    LvglState("Grind", false),
    selectedCoffee(selectedCoffee),
    weight(weight)
{
    // Create container for grind selector
    lv_obj_t* grindContainer = lv_obj_create(root);
    lv_obj_remove_style_all(grindContainer);
    lv_obj_set_size(grindContainer, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_center(grindContainer);
    lv_obj_set_layout(grindContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(grindContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(grindContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(grindContainer, 16, 0);
    lv_obj_remove_flag(grindContainer, LV_OBJ_FLAG_SCROLLABLE);
    
    // Create - button
    lv_obj_t* minusBtn = lv_btn_create(grindContainer);
    lv_obj_set_size(minusBtn, 60, 60);
    lv_obj_add_event_cb(minusBtn, [](lv_event_t* e) {
        GrindState* state = static_cast<GrindState*>(lv_event_get_user_data(e));
        if (state->grindSize > 1) {
            state->grindSize--;
            state->updateGrindLabel();
        }
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* minusLabel = lv_label_create(minusBtn);
    lv_label_set_text(minusLabel, "-");
    lv_obj_set_style_text_font(minusLabel, &lv_font_fira_code_32, 0);
    lv_obj_center(minusLabel);
    
    // Create grind size label
    grindLabel = lv_label_create(grindContainer);
    lv_obj_set_width(grindLabel, 100);
    lv_obj_set_style_text_align(grindLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(grindLabel, &lv_font_fira_code_32, 0);
    updateGrindLabel();
    
    // Create + button
    lv_obj_t* plusBtn = lv_btn_create(grindContainer);
    lv_obj_set_size(plusBtn, 60, 60);
    lv_obj_add_event_cb(plusBtn, [](lv_event_t* e) {
        GrindState* state = static_cast<GrindState*>(lv_event_get_user_data(e));
        state->grindSize++;
        state->updateGrindLabel();
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* plusLabel = lv_label_create(plusBtn);
    lv_label_set_text(plusLabel, "+");
    lv_obj_set_style_text_font(plusLabel, &lv_font_fira_code_32, 0);
    lv_obj_center(plusLabel);
    
    // Create Next button in top right
    lv_obj_t* nextBtn = lv_btn_create(root);
    lv_obj_set_size(nextBtn, 80, 40);
    lv_obj_align(nextBtn, LV_ALIGN_TOP_RIGHT, -LvglState::PADDING, LvglState::PADDING);
    lv_obj_add_event_cb(nextBtn, [](lv_event_t* e) {
        GrindState* state = static_cast<GrindState*>(lv_event_get_user_data(e));
        // State won't exist during state transition, so need to copy
        std::string selectedCoffee = state->selectedCoffee;
        float weight = state->weight;
        int grindSize = state->grindSize;

        state->stateTransition = [selectedCoffee, weight, grindSize]() {
            return new BrewState(selectedCoffee, weight, grindSize, 2.5f);
        };
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* nextLabel = lv_label_create(nextBtn);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);
}

void GrindState::updateGrindLabel() {
    lv_label_set_text_fmt(grindLabel, "%d", grindSize);
}
