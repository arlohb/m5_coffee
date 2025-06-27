#pragma once

#include <lvgl.h>
#include "LvglState.h"
#include "Timer.h"
#include "StatusBar.h"
#include "../scales/ScalesTask.h"

class BrewState : public LvglState {
public:
    BrewState();
    ~BrewState() override = default;

    StateTransition loop() override;

private:
    Timer timer;
    
    ScalesTask scales;
};
