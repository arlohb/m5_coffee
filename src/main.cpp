#include <M5Core2.h>

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("Hello world");
}

void loop() {
    delay(10);
    M5.update();

    Serial.println(M5.BtnA.isPressed());
}

