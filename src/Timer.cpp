#include "Timer.h"

#include <fmt/core.h>
#include "Utils.h"

Timer::Timer() {
    auto root = LObject(lv_scr_act());
    label = LLabel(root);
    label.setText("00.0s");
    label.align(LV_ALIGN_CENTER, 0, 0);

    startTime = std::chrono::steady_clock::now();
}

void Timer::update() {
    if (!running) return;

    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

    label.setText(fmt::format("{:02}.{:01}s", ms / 1000, ms % 1000 / 100));
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
