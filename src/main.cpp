#include <fmt/core.h>

#include <M5Core2.h>

#include "Utils.h"

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(3);
    // From 2500 - 3300
    M5.Axp.SetLcdVoltage(3300);
}

void loop() {
    delay(10);
    M5.update();

    if (M5.Touch.ispressed()) {
        Point touch = M5.Touch.getPressPoint();

        M5.Lcd.drawCircle(touch.x, touch.y, 10, rgb(0x4000FF));
    }

    for (int x = 0; x < 64; x++)
        for (int y = 0; y < 64; y++)
            M5.Lcd.drawPixel(x, y, rgb(x * 4, 0, y * 4));

    M5.Lcd.drawString(fmt::format("{}", M5.BtnA.isPressed()).c_str(), 0, 100);
}

