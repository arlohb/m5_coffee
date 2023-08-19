#pragma once

#include "LButton.h"
#include "LLabel.h"
#include "Utils.h"

class Program {
    private:
        inline static const int width = 320;
        inline static const int height = 240;

        lv_disp_draw_buf_t drawBuffer;
        // I do not know what the / 10 is for
        lv_color_t colourBuffer[width * height / 10];

        lv_disp_drv_t displayDriver;
        lv_indev_drv_t indevDriver;

        LObject root;
        LButton btn;
        LLabel label;
        LLabel timeLabel;

        void SetupDisplay();
        void SetupInput();
        void SetupNetwork();

    public:
        Program();

        void Loop();

        void SetBrightness(uint8_t brightness);
};

