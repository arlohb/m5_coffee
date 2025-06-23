#pragma once

#include <lvgl.h>
#include "State.h"
#include "Timer.h"

class MainState : public State {
public:
    MainState();

    void loop() override;

private:
    lv_obj_t* timeLabel;
    lv_obj_t* batteryLabel;
    
    Timer timer;
};
