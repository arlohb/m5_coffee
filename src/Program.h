#pragma once

#include <memory>
#include "Utils.h"
#include "Timer.h"
#include "states/State.h"

class Program {
private:
    inline static const int width = 320;
    inline static const int height = 240;

    lv_display_t* display;
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[width * height / 10];

    lv_indev_t* indev;
    
    std::unique_ptr<State> state;

    void setupDisplay();
    void setupInput();
    void setupTheme();
    static void setupNetworkTask(void* arg);

public:
    Program();

    void loop();
};

