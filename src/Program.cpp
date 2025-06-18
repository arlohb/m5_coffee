#include "Program.h"

#include <WiFi.h>
#include <M5Unified.h>
#include <M5UnitLCD.h>
#include "Secrets.h"

void Program::SetupDisplay() {
    lv_init();
    display = lv_display_create(width, height);
    lv_display_set_buffers(display, colourBuffer, nullptr, sizeof(colourBuffer), LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_flush_cb(
        display,
        [] (
            lv_display_t* display,
            const lv_area_t* area,
            unsigned char* colour
        ) {
            uint32_t w = area->x2 - area->x1 + 1;
            uint32_t h = area->y2 - area->y1 + 1;

            M5.Display.startWrite();
            M5.Display.setAddrWindow(area->x1, area->y1, w, h);
            M5.Display.writePixels((uint16_t*)colour, w * h, true);
            M5.Display.endWrite();

            lv_display_flush_ready(display);
        }
    );
}

void Program::SetupInput() {
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(
        indev,
        [] (lv_indev_t* indev, lv_indev_data_t* data) {
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
        }
    );
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
    btn.SetLabel("0");
    btn.SetPos(10, 10);
    btn.SetSize(120, 100);
    btn.OnPress<LLabel>([](LLabel* label) {
        dbgln("Btn pressed");
        int prev = std::stoi(label->GetText());
        label->SetText(std::to_string(prev + 1));
    }, &*btn.label);

    timeLabel = LLabel(root);
    timeLabel.Align(LV_ALIGN_BOTTOM_LEFT, 0, -20);
    
    timer = Timer();
    
    LButton playBtn(root);
    playBtn.SetLabel("Play");
    playBtn.SetSize(80, 80);
    playBtn.Align(LV_ALIGN_CENTER, 50, 70);
    playBtn.OnPress<Timer>([](Timer* timer) {
        dbgln("Play button pressed");
        if (timer->isRunning()) {
            timer->stop();
        } else {
            timer->start();
        }
    }, &*timer);
    
    LButton resetBtn(root);
    resetBtn.SetLabel("Reset");
    resetBtn.SetSize(80, 80);
    resetBtn.Align(LV_ALIGN_CENTER, -50, 70);
    resetBtn.OnPress<Timer>([](Timer* timer) {
        dbgln("Reset button pressed");
        timer->reset();
    }, &*timer);
}

void Program::Loop() {
    M5.update();

    lv_tick_inc(1);
    lv_task_handler();
    lv_timer_handler();

    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    timeLabel.SetText(fmt::format("{:02}:{:02}:{:02}", time.hours, time.minutes, time.seconds));
    
    timer->update();

    delay(1);
}

