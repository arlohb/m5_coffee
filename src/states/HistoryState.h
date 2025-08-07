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
    HistoryState(const std::string& selectedCoffee);

    std::optional<StateTransition> loop() override;

private:
    lv_obj_t* table;
    
    std::string selectedCoffee;
    
    /// Is only true after brews are loaded and before table is filled.
    std::atomic<bool> brewsLoaded = false;
    std::vector<Brew> brews;
};
