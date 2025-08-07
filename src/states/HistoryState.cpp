#include "HistoryState.h"

#include <fmt/core.h>
#include <WiFi.h>
#include "ScalesState.h"
#include "BrewState.h"
#include "../Utils.h"

HistoryState::HistoryState(const std::string& selectedCoffee) :
    LvglState("History", false),
    selectedCoffee(selectedCoffee)
{
    lv_obj_t* nextBtn = lv_btn_create(root);
    lv_obj_align(nextBtn, LV_ALIGN_TOP_RIGHT, -4, 4);
    lv_obj_set_size(nextBtn, 100, 40);
    lv_obj_add_event_cb(nextBtn, [](lv_event_t* e) {
        LOG_INFO("Next button clicked");
        HistoryState* state = static_cast<HistoryState*>(lv_event_get_user_data(e));
        
        std::string selectedCoffee = state->selectedCoffee;
        state->stateTransition = [=]() {
            return new ScalesState([selectedCoffee](float weight) {
                return [selectedCoffee, weight]() {
                    return new BrewState(selectedCoffee, weight);
                };
            });
        };
    }, LV_EVENT_CLICKED, this);
    
    lv_obj_t* nextLabel = lv_label_create(nextBtn);
    lv_label_set_text(nextLabel, "Next");
    lv_obj_center(nextLabel);

    table = lv_table_create(root);
    lv_obj_align(table, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_font(table, &lv_font_fira_code_14, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(table, 5, LV_PART_ITEMS);
    lv_obj_set_size(table, lv_pct(100), 160);
    
    const int colWidth = LV_HOR_RES / 5 - 2;
    lv_table_set_col_cnt(table, 5);
    lv_table_set_cell_value(table, 0, 0, "In (g)");
    lv_table_set_col_width(table, 0, colWidth);
    lv_table_set_cell_value(table, 0, 1, "Ratio");
    lv_table_set_col_width(table, 1, colWidth);
    lv_table_set_cell_value(table, 0, 2, "Aim Out (g)");
    lv_table_set_col_width(table, 2, colWidth);
    lv_table_set_cell_value(table, 0, 3, "Actual Out (g)");
    lv_table_set_col_width(table, 3, colWidth);
    lv_table_set_cell_value(table, 0, 4, "Brew Time (s)");
    lv_table_set_col_width(table, 4, colWidth);

    // Start loading brews
    xTaskCreate([](void* arg) {
        LOG_INFO("Loading brews");

        HistoryState* state = static_cast<HistoryState*>(arg);
        state->brews = CoffeeDB::get(state->selectedCoffee);
        state->brewsLoaded = true;
        
        LOG_INFO("Brews loaded: {}", state->brews.size());
        
        vTaskDelete(nullptr);
    }, "get_brews", 8 * 1024, this, 1, nullptr);
}

std::optional<StateTransition> HistoryState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;
    
    if (brewsLoaded) {
        brewsLoaded = false;
        
        lv_table_set_row_count(table, brews.size() + 1);
        for (size_t i = 0; i < brews.size(); ++i) {
            // Reverse order
            int row = brews.size() - i;

            lv_table_set_cell_value(table, row, 0, fmt::format("{:g}", brews[i].in).c_str());
            lv_table_set_cell_value(table, row, 1, fmt::format("{:g}", brews[i].ratio).c_str());
            lv_table_set_cell_value(table, row, 2, fmt::format("{:g}", brews[i].aimOut).c_str());
            lv_table_set_cell_value(table, row, 3, fmt::format("{:g}", brews[i].actualOut).c_str());
            lv_table_set_cell_value(table, row, 4, fmt::format("{:g}", brews[i].brewTime).c_str());
        }
    }

    return std::nullopt;
}
