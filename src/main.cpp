#include <fmt/core.h>
#include <M5Core2.h>
#include <lvgl.h>
#include "Utils.h"

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t drawBuffer;
static lv_color_t colourBuffer[screenWidth * screenHeight / 10];

TFT_eSPI tft(screenWidth, screenHeight);

void SetupDisplay() {
    lv_disp_draw_buf_init(&drawBuffer, colourBuffer, nullptr, screenWidth * screenHeight / 10);

    static lv_disp_drv_t displayDriver;
    lv_disp_drv_init(&displayDriver);

    displayDriver.hor_res = screenWidth;
    displayDriver.ver_res = screenHeight;

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

void SetupInput() {
    static lv_indev_drv_t indevDriver;
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

void setup() {
    M5.begin();
    // From 2500 - 3300
    M5.Axp.SetLcdVoltage(3300);

    tft.begin();
    tft.setRotation(1);

    lv_init();

    SetupDisplay();
    SetupInput();

    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_pos(btn, 10, 10);
    lv_obj_set_size(btn, 120, 100);
    lv_obj_add_event_cb(btn, [](lv_event_t* e){
        dbgln("Btn pressed");
    }, LV_EVENT_PRESSED, NULL);

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, "hello");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
    M5.update();

    lv_tick_inc(1);
    lv_task_handler();
    lv_timer_handler();

    delay(1);
}

