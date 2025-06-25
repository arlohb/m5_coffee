#include "Timer.h"

#include <fmt/core.h>
#include "Utils.h"

Timer::Timer() {
    auto root = lv_scr_act();
    btn = lv_btn_create(root);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn, 120, 80);
    
    label = lv_label_create(btn);
    lv_label_set_text(label, "00.0");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_set_style_text_font(label, &lv_font_fira_code_32, LV_STATE_DEFAULT);

    startTime = std::chrono::steady_clock::now();
}

Timer::~Timer() {
    lv_obj_delete(btn);
}

void Timer::registerOnPress() {
    lv_obj_add_event_cb(btn, [] (lv_event_t* e) {
        Timer* timer = (Timer*)lv_event_get_user_data(e);

        if (timer->isRunning()) {
            timer->stop();
        } else {
            timer->start();
        }

        M5.Power.setVibration(255);
        M5.Speaker.tone(1000, 60);

        xTimerStart(xTimerCreate(
            "VibrationOff",
            pdMS_TO_TICKS(30),
            pdFALSE,
            nullptr,
            [] (TimerHandle_t) {
                M5.Power.setVibration(0);
            }
        ), 0);
    }, LV_EVENT_PRESSED, this);
}

void Timer::update() {
    if (!running) return;

    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

    lv_label_set_text(label, fmt::format("{:02}.{:01}", ms / 1000, ms % 1000 / 100).c_str());
}

void Timer::start() {
    if (running) return;

    startTime = std::chrono::steady_clock::now();
    running = true;
}

void Timer::stop() {
    if (!running) return;
    running = false;
}
