#pragma once

#include <lvgl.h>
#include "State.h"
#include "Timer.h"
#include "StatusBar.h"

class MainState : public State {
public:
    MainState();

    void loop() override;

private:
    StatusBar statusBar;

    Timer timer;
};
