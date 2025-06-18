#pragma once

#include <chrono>
#include "LLabel.h"

class Timer {
public:
    Timer();
    
    void update();
    void reset();
    void start();
    void stop();
    bool isRunning() const { return running; }

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool running = false;

    LLabel label;
};
