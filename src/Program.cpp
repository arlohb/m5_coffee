#include "Program.h"

#include <WiFi.h>
#include <M5Unified.h>
#include <M5UnitLCD.h>
#include "Secrets.h"

void Program::SetupDisplay() {
    lv_init();
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

        M5.Display.startWrite();
        M5.Display.setAddrWindow(area->x1, area->y1, w, h);
        M5.Display.pushPixels((uint16_t*)&colour->full, w * h, true);
        M5.Display.endWrite();

        lv_disp_flush_ready(displayDriver);
    };

    displayDriver.draw_buf = &drawBuffer;

    lv_disp_drv_register(&displayDriver);
}

void Program::SetupInput() {
    lv_indev_drv_init(&indevDriver);

    indevDriver.type = LV_INDEV_TYPE_POINTER;

    indevDriver.read_cb = [](lv_indev_drv_t* indevDriver, lv_indev_data_t* data) {
        int count = M5.Touch.getCount();

        if (count > 0) {
            data->state = LV_INDEV_STATE_PRESSED;

            // LVGL doesn't support multi touch
            // Just use the last point
            auto point = M5.Touch.getTouchPointRaw(count - 1);
            data->point.x = point.x;
            data->point.y = point.y;

        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    };

    lv_indev_drv_register(&indevDriver);
}

void Program::SetupNetwork() {
    WiFi.begin(secrets::ssid, secrets::password);

    while (WiFi.status() != WL_CONNECTED)
        delay(100);

    dbgln("Getting NTP time...");

    configTime(0, 3600, "pool.ntp.org", "time.nist.gov");

    tm cTime;
    getLocalTime(&cTime);

    m5::rtc_time_t rtcTime;
    rtcTime.hours = cTime.tm_hour;
    rtcTime.minutes = cTime.tm_min;
    rtcTime.seconds = cTime.tm_sec;
    M5.Rtc.setTime(rtcTime);
}

Program::Program() {
    auto config = M5.config();
    config.fallback_board = m5::board_t::board_M5StackCore2;
    M5.begin(config);

    dbgln("Setup M5");

    dbgln("Setting up lvgl display...");
    SetupDisplay();
    dbgln("Setting up lvgl input...");
    SetupInput();
    dbgln("Connecting to WiFi...");
    SetupNetwork();
    dbgln("Setup done");

    root = LObject(lv_scr_act());

    btn = LButton(root);
    btn.SetPos(10, 10);
    btn.SetSize(120, 100);
    btn.OnPress([&]() {
        dbgln("Btn pressed");
        int prev = std::stoi(label.GetText());
        label.SetText(std::to_string(prev + 1));
    });

    label = LLabel(btn);
    label.SetText("0");
    label.Align(LV_ALIGN_CENTER, 0, 0);

    timeLabel = LLabel(root);
    timeLabel.Align(LV_ALIGN_BOTTOM_LEFT, 0, -20);
}

void Program::Loop() {
    M5.update();

    lv_tick_inc(1);
    lv_task_handler();
    lv_timer_handler();

    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    timeLabel.SetText(fmt::format("{:02}:{:02}:{:02}", time.hours, time.minutes, time.seconds));

    delay(1);
}

