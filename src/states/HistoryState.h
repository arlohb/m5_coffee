#pragma once

#include <lvgl.h>
#include <atomic>
#include <vector>
#include <string>
#include "LvglState.h"
#include "Timer.h"
#include "StatusBar.h"
#include "../CoffeeDB.h"

class HistoryState : public LvglState {
public:
    HistoryState();

    StateTransition loop() override;

private:
    lv_obj_t* coffeeSelector = nullptr;
    lv_obj_t* table;
    
    /// Stays true after coffees are loaded.
    std::atomic<bool> coffeesLoaded = false;
    std::vector<std::string> coffees;
    std::string selectedCoffee;
    
    /// Is only true after brews are loaded and before table is filled.
    std::atomic<bool> brewsLoaded = false;
    std::vector<Brew> brews;
    
    void setupSelector();
};
