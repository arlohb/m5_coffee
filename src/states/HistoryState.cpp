#include "HistoryState.h"

#include <M5Unified.h>

HistoryState::HistoryState() : LvglState("History") {
}

State* HistoryState::loop() {
    State* newState = LvglState::loop();
    if (newState) return newState;
    
    return nullptr;
}
