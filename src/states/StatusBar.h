#pragma once

#include <lvgl.h>

class StatusBar {
public:
    StatusBar();
    ~StatusBar();

    void loop();

private:
    lv_obj_t* bar;

    lv_obj_t* batteryLabel;
    lv_obj_t* timeLabel;
    
    lv_obj_t* offBtn;
};
