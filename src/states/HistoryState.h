#pragma once

#include <lvgl.h>
#include "LvglState.h"
#include "Timer.h"
#include "StatusBar.h"

class HistoryState : public LvglState {
public:
    HistoryState();

    State* loop() override;

private:
    lv_obj_t* table;
};
