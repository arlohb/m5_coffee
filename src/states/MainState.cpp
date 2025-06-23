#include "MainState.h"

#include <M5Unified.h>

MainState::MainState() : LvglState("Main") {
    timer.registerOnPress();
}

State* MainState::loop() {
    State* newState = LvglState::loop();
    if (newState) return newState;

    timer.update();
    
    return nullptr;
}
