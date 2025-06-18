#pragma once

#include "LButton.h"
#include "LLabel.h"
#include "Utils.h"
#include "Timer.h"

class Program {
private:
    inline static const int width = 320;
    inline static const int height = 240;

    lv_display_t* display;
    // Isn't the full size of the screen as it is drawn partially
    lv_color_t colourBuffer[width * height / 10];

    lv_indev_t* indev;

    LObject root;
    LLabel timeLabel;
    
    std::optional<Timer> timer;

    void SetupDisplay();
    void SetupInput();
    void SetupNetwork();

public:
    Program();

    void Loop();
};

