#pragma once

#include <lvgl.h>
#include "LvglState.h"
#include "Timer.h"
#include "StatusBar.h"

class BrewState : public LvglState {
public:
    BrewState();
    ~BrewState() override = default;

    State* loop() override;

private:
    Timer timer;
};
