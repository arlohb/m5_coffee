#include "BrewState.h"

#include <M5Unified.h>

BrewState::BrewState() : LvglState("Brew") {
    timer.registerOnPress();
}

StateTransition BrewState::loop() {
    StateTransition stateTransition = LvglState::loop();
    if (stateTransition) return stateTransition;

    timer.update();
    
    return std::nullopt;
}
