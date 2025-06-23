#include "MainState.h"

#include <M5Unified.h>

MainState::MainState() : LvglState("Main") {
    timer.registerOnPress();
}

void MainState::loop() {
    LvglState::loop();
    timer.update();
}
