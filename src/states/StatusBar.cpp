#include "StatusBar.h"

#include <fmt/core.h>
#include <M5Unified.h>

StatusBar::StatusBar() {
    lv_obj_t* root = lv_scr_act();
    
    bar = lv_obj_create(root);
    lv_obj_set_size(bar, LV_HOR_RES, HEIGHT);
    lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_side(bar, LV_BORDER_SIDE_NONE, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(bar, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(bar, 0, LV_STATE_DEFAULT);

    timeLabel = lv_label_create(bar);
    lv_obj_align(timeLabel, LV_ALIGN_LEFT_MID, 16, 0);

    batteryLabel = lv_label_create(bar);
    lv_obj_align(batteryLabel, LV_ALIGN_RIGHT_MID, -48, 0);

    offBtn = lv_btn_create(bar);
    lv_obj_align(offBtn, LV_ALIGN_RIGHT_MID, -2, 0);
    lv_obj_set_size(offBtn, 28, 28);
    lv_obj_add_event_cb(offBtn, [] (lv_event_t* e) {
        M5.Power.deepSleep(0, true);
    }, LV_EVENT_PRESSED, nullptr);
    lv_obj_set_style_bg_color(offBtn, lv_theme_get_color_secondary(root), LV_STATE_DEFAULT);
    
    lv_obj_t* offBtnLabel = lv_label_create(offBtn);
    lv_label_set_text(offBtnLabel, "x");
    lv_obj_align(offBtnLabel, LV_ALIGN_CENTER, 0, 0);
}

StatusBar::~StatusBar() {
    lv_obj_delete(bar);
}

void StatusBar::loop() {
    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    lv_label_set_text(timeLabel, fmt::format("{:02}:{:02}:{:02}", time.hours, time.minutes, time.seconds).c_str());

    static auto icons = std::array<const char*, 5>{
        LV_SYMBOL_BATTERY_EMPTY,
        LV_SYMBOL_BATTERY_1,
        LV_SYMBOL_BATTERY_2,
        LV_SYMBOL_BATTERY_3,
        LV_SYMBOL_BATTERY_FULL
    };
    
    auto icon = icons[std::clamp(M5.Power.getBatteryLevel() / 20, 0, 4)];
    
    lv_label_set_text(batteryLabel, fmt::format("{}% {}{}",
        M5.Power.getBatteryLevel(),
        M5.Power.isCharging() ? LV_SYMBOL_CHARGE : "",
        icon
    ).c_str());
}
