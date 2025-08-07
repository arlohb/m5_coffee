#pragma once

#include "LvglState.h"
#include <atomic>

class MenuState : public LvglState {
public:
    MenuState();
    
    std::optional<StateTransition> loop() override;

private:
    std::atomic<bool> coffeesLoaded = false;
    std::vector<std::string> coffees;
    std::string selectedCoffee;
};
