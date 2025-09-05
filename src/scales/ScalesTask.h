#pragma once

#include <M5Unified.h>
#include <atomic>
#include "Scales.h"

class ScalesTask {
public:
    ScalesTask();
    ~ScalesTask();
    
    float getWeight() const;
    void zero();

private:
    TaskHandle_t taskHandle = nullptr;
    
    std::unique_ptr<Scales> scales;
    std::atomic<float> weight = 0.0f;
};
