#include "Program.h"

#include <WiFi.h>
#include <M5Unified.h>
#include <M5UnitLCD.h>
#include "Secrets.h"

void Program::setupDisplay() {
    lv_init();
    lv_tick_set_cb(xTaskGetTickCount);
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

void Program::setupInput() {
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

void Program::setupNetwork() {
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
    setupDisplay();
    dbgln("Setting up lvgl input...");
    setupInput();
    dbgln("Connecting to WiFi...");
    setupNetwork();
    dbgln("Setup done");

    root = LObject(lv_scr_act());

    timeLabel = LLabel(root);
    timeLabel.align(LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_set_style_text_font(timeLabel.ptr, &lv_font_fira_code_20, LV_STATE_DEFAULT);
    
    timer = Timer();
    timer->registerOnPress();
    
    batteryLabel = LLabel(root);
    batteryLabel.align(LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_text_font(batteryLabel.ptr, &lv_font_fira_code_20, LV_STATE_DEFAULT);
}

void Program::loop() {
    M5.update();

    lv_timer_handler();

    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    timeLabel.setText(fmt::format("{:02}:{:02}:{:02}", time.hours, time.minutes, time.seconds));
    
    timer->update();
    
    static auto icons = std::array<const char*, 5>{
        LV_SYMBOL_BATTERY_EMPTY,
        LV_SYMBOL_BATTERY_1,
        LV_SYMBOL_BATTERY_2,
        LV_SYMBOL_BATTERY_3,
        LV_SYMBOL_BATTERY_FULL
    };
    
    auto icon = icons[std::clamp(M5.Power.getBatteryLevel() / 20, 0, 4)];
    
    batteryLabel.setText(fmt::format("{}{} {}%",
        icon,
        M5.Power.isCharging() ? LV_SYMBOL_CHARGE : "",
        M5.Power.getBatteryLevel()
    ));

    delay(1);
}

