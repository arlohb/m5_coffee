#include "MainState.h"

#include <fmt/core.h>
#include <M5Unified.h>

MainState::MainState() {
    lv_obj_t* root = lv_scr_act();

    timeLabel = lv_label_create(root);
    lv_obj_align(timeLabel, LV_ALIGN_TOP_LEFT, 20, 20);
    lv_obj_set_style_text_font(timeLabel, &lv_font_fira_code_20, LV_STATE_DEFAULT);

    batteryLabel = lv_label_create(root);
    lv_obj_align(batteryLabel, LV_ALIGN_BOTTOM_LEFT, 20, -20);
    lv_obj_set_style_text_font(batteryLabel, &lv_font_fira_code_20, LV_STATE_DEFAULT);
    
    lv_obj_t* offBtn = lv_btn_create(root);
    lv_obj_align(offBtn, LV_ALIGN_TOP_RIGHT, -12, 12);
    lv_obj_set_size(offBtn, 48, 48);
    lv_obj_add_event_cb(offBtn, [] (lv_event_t* e) {
        M5.Power.deepSleep(0, true);
    }, LV_EVENT_PRESSED, nullptr);
    lv_obj_set_style_bg_color(offBtn, lv_theme_get_color_secondary(root), LV_STATE_DEFAULT);
    
    lv_obj_t* offBtnLabel = lv_label_create(offBtn);
    lv_label_set_text(offBtnLabel, LV_SYMBOL_POWER);
    lv_obj_align(offBtnLabel, LV_ALIGN_CENTER, 0, 0);
    
    timer.registerOnPress();
}

void MainState::loop() {
    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    lv_label_set_text(timeLabel, fmt::format("{:02}:{:02}:{:02}", time.hours, time.minutes, time.seconds).c_str());
    
    timer.update();
    
    static auto icons = std::array<const char*, 5>{
        LV_SYMBOL_BATTERY_EMPTY,
        LV_SYMBOL_BATTERY_1,
        LV_SYMBOL_BATTERY_2,
        LV_SYMBOL_BATTERY_3,
        LV_SYMBOL_BATTERY_FULL
    };
    
    auto icon = icons[std::clamp(M5.Power.getBatteryLevel() / 20, 0, 4)];
    
    lv_label_set_text(batteryLabel, fmt::format("{}{} {}%",
        icon,
        M5.Power.isCharging() ? LV_SYMBOL_CHARGE : "",
        M5.Power.getBatteryLevel()
    ).c_str());
}
