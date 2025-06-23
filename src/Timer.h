#pragma once

#include <chrono>
#include "LButton.h"

class Timer {
public:
    Timer();

    // Register the button press callback.
    // Called separately from the constructor because it relies on the address of this,
    // Which may be moved after the constructor.
    void registerOnPress();

    void update();
    void start();
    void stop();
    bool isRunning() const { return running; }

private:
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    bool running = false;

public:
    LButton btn;
    LLabel label;
};
