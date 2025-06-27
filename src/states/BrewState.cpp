#include "BrewState.h"

#include <M5Unified.h>

BrewState::BrewState() : LvglState("Brew") {
    timer.registerOnPress();
}

State* BrewState::loop() {
    State* newState = LvglState::loop();
    if (newState) return newState;

    timer.update();
    
    return nullptr;
}
