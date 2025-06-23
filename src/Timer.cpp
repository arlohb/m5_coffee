#include "Timer.h"

#include <fmt/core.h>
#include "Utils.h"

Timer::Timer() {
    auto root = LObject(lv_scr_act());
    btn = LButton(root);
    btn.align(LV_ALIGN_CENTER, 0, 0);
    btn.setSize(120, 80);
    
    label = LLabel(btn);
    label.setText("00.0");
    label.align(LV_ALIGN_LEFT_MID, 8, 0);

    startTime = std::chrono::steady_clock::now();
}

void Timer::registerOnPress() {
    btn.onPress<Timer>([](Timer* timer) {
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
    }, this);
}

void Timer::update() {
    if (!running) return;

    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

    label.setText(fmt::format("{:02}.{:01}", ms / 1000, ms % 1000 / 100));
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
