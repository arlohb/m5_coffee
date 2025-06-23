#pragma once

#include <lvgl.h>
#include "LvglState.h"
#include "Timer.h"
#include "StatusBar.h"

class MainState : public LvglState {
public:
    MainState();
    ~MainState() override = default;

    void loop() override;

private:
    Timer timer;
};
