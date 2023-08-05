#include "Program.h"

void Program::SetupDisplay() {
    // This is only used inside this function
    // This can't be in the class as it can't be captured in the lambda
    static TFT_eSPI tft(width, height);
    tft.begin();
    tft.setRotation(1);

    lv_disp_draw_buf_init(&drawBuffer, colourBuffer, nullptr, width * height / 10);

    lv_disp_drv_init(&displayDriver);

    displayDriver.hor_res = width;
    displayDriver.ver_res = height;

    displayDriver.flush_cb = [](
        lv_disp_drv_t* displayDriver,
        const lv_area_t* area,
        lv_color_t* colour
    ) {
        uint32_t w = area->x2 - area->x1 + 1;
        uint32_t h = area->y2 - area->y1 + 1;

        tft.startWrite();
        tft.setAddrWindow(area->x1, area->y1, w, h);
        tft.pushColors((uint16_t*)&colour->full, w * h, true);
        tft.endWrite();

        lv_disp_flush_ready(displayDriver);
    };

    displayDriver.draw_buf = &drawBuffer;

    lv_disp_drv_register(&displayDriver);
}

void Program::SetupInput() {
    lv_indev_drv_init(&indevDriver);

    indevDriver.type = LV_INDEV_TYPE_POINTER;

    indevDriver.read_cb = [](lv_indev_drv_t* indevDriver, lv_indev_data_t* data) {
        if (M5.Touch.ispressed()) {
            data->state = LV_INDEV_STATE_PRESSED;
            Point pos = M5.Touch.getPressPoint();
            data->point.x = pos.x;
            data->point.y = pos.y;
        } else
            data->state = LV_INDEV_STATE_RELEASED;
    };

    lv_indev_drv_register(&indevDriver);
}

Program::Program() {
    M5.begin();

    lv_init();

    SetupDisplay();
    SetupInput();

    root = LObject(lv_scr_act());

    btn = LButton(root);
    btn.SetPos(10, 10);
    btn.SetSize(120, 100);
    btn.OnPress([]() {
        dbgln("Btn pressed");
    });

    label = LLabel(btn);
    label.SetText("Hello");
    label.Align(LV_ALIGN_CENTER, 0, 0);
}

void Program::Loop() {
    M5.update();

    lv_tick_inc(1);
    lv_task_handler();
    lv_timer_handler();

    delay(1);
}

void Program::SetBrightness(uint8_t brightness) {
    // From 2500 - 3300
    M5.Axp.SetLcdVoltage(2500 + (brightness * 800 / 255));
}

