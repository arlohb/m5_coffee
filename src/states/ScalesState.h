#pragma once

#include <atomic>
#include "LvglState.h"
#include "../scales/Scales.h"

class ScalesState : public LvglState {
public:
    ScalesState();
    ~ScalesState() override;

    State* loop() override;

private:
    static const bool calibrateOnStart = false;

    Scales scales;
    std::atomic<float> weight = 0.0f;
    
    TaskHandle_t loopTask = nullptr;
    
    lv_obj_t* weightLabel;
    
    void calibrate();
};
