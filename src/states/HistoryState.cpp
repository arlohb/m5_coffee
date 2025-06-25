#include "HistoryState.h"

#include <fmt/core.h>
#include "../CoffeeDB.h"

HistoryState::HistoryState() : LvglState("History", false) {
    lv_obj_t* coffeSelector = lv_dropdown_create(root);
    lv_obj_align(coffeSelector, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_dropdown_clear_options(coffeSelector);
    for (const auto& coffee : CoffeeDB::getCoffees()) {
        lv_dropdown_add_option(coffeSelector, coffee.c_str(), LV_DROPDOWN_POS_LAST);
    }
    
    table = lv_table_create(root);
    lv_obj_align(table, LV_ALIGN_BOTTOM_MID, 0, -32);
    lv_obj_set_style_text_font(table, &lv_font_fira_code_14, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(table, 5, LV_PART_ITEMS);
    lv_obj_set_size(table, lv_pct(100), lv_pct(60));
    
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
    
    char selectedCoffee[64];
    lv_dropdown_get_selected_str(coffeSelector, selectedCoffee, sizeof(selectedCoffee));
    std::vector<Brew> brews = CoffeeDB::get(selectedCoffee);
    
    for (size_t i = 0; i < brews.size(); ++i) {
        lv_table_set_cell_value(table, i+1, 0, fmt::format("{:g}", brews[i].in).c_str());
        lv_table_set_cell_value(table, i+1, 1, fmt::format("{:g}", brews[i].ratio).c_str());
        lv_table_set_cell_value(table, i+1, 2, fmt::format("{:g}", brews[i].aimOut).c_str());
        lv_table_set_cell_value(table, i+1, 3, fmt::format("{:g}", brews[i].actualOut).c_str());
        lv_table_set_cell_value(table, i+1, 4, fmt::format("{:g}", brews[i].brewTime).c_str());
    }
}

State* HistoryState::loop() {
    State* newState = LvglState::loop();
    if (newState) return newState;
    
    return nullptr;
}
