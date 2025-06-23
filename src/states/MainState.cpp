#include "MainState.h"

#include <M5Unified.h>

MainState::MainState() {
    timer.registerOnPress();
}

void MainState::loop() {
    statusBar.loop();
    timer.update();
}
