#include "MenuState.h"

#include "HistoryState.h"
#include "ScalesState.h"
#include "../Utils.h"
#include <WiFi.h>

std::atomic<bool> MenuState::isMenuLoaded = false;

MenuState::MenuState() : LvglState("m5_coffee", false) {
    isMenuLoaded = true;

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
        // State won't exist during state transition, so need to copy
        std::string selectedCoffee = state->selectedCoffee;

        state->stateTransition = [selectedCoffee]() {
            return new HistoryState(selectedCoffee);
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

    xTaskCreate([](void* arg) {
        // Wait for WiFi to connect
        while (WiFi.status() != WL_CONNECTED) delay(100);
        
        std::vector<std::string> coffees = CoffeeDB::getCoffees();
        
        // If menu has unloaded, exit
        if (!MenuState::isMenuLoaded) {
            vTaskDelete(nullptr);
            return;
        }

        LOG_INFO("Loading coffees...");
        MenuState* state = static_cast<MenuState*>(arg);
        state->coffees = coffees;
        state->coffeesLoaded = true;
        LOG_INFO("Coffees loaded: {}", state->coffees.size());
        
        vTaskDelete(nullptr);
    }, "get_coffees", 8 * 1024, this, 1, nullptr);
}

MenuState::~MenuState() {
    isMenuLoaded = false;
}

std::optional<StateTransition> MenuState::loop() {
    auto stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;
    
    if (coffeesLoaded) {
        coffeesLoaded = false;
        
        lv_obj_t* coffeeSelector = lv_dropdown_create(root);
        lv_obj_align(coffeeSelector, LV_ALIGN_BOTTOM_LEFT, LvglState::PADDING, -LvglState::PADDING);
        lv_obj_set_size(coffeeSelector, LV_HOR_RES - 2 * LvglState::PADDING, LV_SIZE_CONTENT);
        lv_dropdown_set_symbol(coffeeSelector, "");
        lv_dropdown_clear_options(coffeeSelector);
        for (const auto& coffee : coffees) {
            lv_dropdown_add_option(coffeeSelector, coffee.c_str(), LV_DROPDOWN_POS_LAST);
        }
        
        lv_obj_add_event_cb(coffeeSelector, [](lv_event_t* e) {
            LOG_INFO("Coffee selected");

            MenuState* state = static_cast<MenuState*>(lv_event_get_user_data(e));
            
            char selectedCoffee[64];
            lv_dropdown_get_selected_str(
                static_cast<lv_obj_t*>(lv_event_get_target(e)),
                selectedCoffee, sizeof(selectedCoffee)
            );
            state->selectedCoffee = selectedCoffee;
        }, LV_EVENT_VALUE_CHANGED, this);
    }

    return std::nullopt;
}
