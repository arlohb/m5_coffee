#include "Program.h"

#include <WiFi.h>
#include <M5Unified.h>
#include <M5UnitLCD.h>
#include "Secrets.h"
#include "states/LoadingState.h"
#include "states/BrewState.h"

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
    // TODO: Should be buffering the input
    // Could start a task to do this
    // https://docs.lvgl.io/master/details/main-modules/indev.html#buffered-reading
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

void Program::setupTheme() {
    lv_theme_t* theme = lv_theme_default_init(
        display,
        lv_palette_main(LV_PALETTE_PINK),
        lv_palette_main(LV_PALETTE_LIGHT_BLUE),
        true,
        &lv_font_fira_code_20
    );
    lv_display_set_theme(display, theme);
}

void Program::setupNetworkTask(void* arg) {
    WiFi.begin(secrets::ssid, secrets::password);

    while (WiFi.status() != WL_CONNECTED)
        delay(100);

    LOG_INFO("Getting NTP time...");

    configTime(0, 3600, "pool.ntp.org", "time.nist.gov");

    tm cTime;
    getLocalTime(&cTime);

    m5::rtc_time_t rtcTime;
    rtcTime.hours = cTime.tm_hour;
    rtcTime.minutes = cTime.tm_min;
    rtcTime.seconds = cTime.tm_sec;
    M5.Rtc.setTime(rtcTime);
    
    vTaskDelete(nullptr);
}

Program::Program() :
    state(std::make_unique<LoadingState>())
{
    auto config = M5.config();
    config.fallback_board = m5::board_t::board_M5StackCore2;
    M5.begin(config);

    LOG_INFO("Starting WiFi task...");
    xTaskCreate(setupNetworkTask, "WiFiTask", 4096, nullptr, 1, nullptr);

    LOG_INFO("Setup M5");

    LOG_INFO("Setting up lvgl display...");
    setupDisplay();
    LOG_INFO("Setting up lvgl input...");
    setupInput();
    LOG_INFO("Setting up lvgl theme...");
    setupTheme();
    LOG_INFO("Setup done");

    state = std::make_unique<BrewState>();
}

void Program::loop() {
    M5.update();

    lv_timer_handler();

    StateTransition stateTransition = state->loop();
    if (stateTransition) {
        LOG_INFO("Transitioning to new state...");
        LOG_INFO("Deleting old state...");
        state.reset();
        LOG_INFO("Creating new state...");
        state = std::unique_ptr<State>(stateTransition.value()());
    }

    delay(1);
}

