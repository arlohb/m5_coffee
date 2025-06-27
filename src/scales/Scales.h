// https://github.com/m5stack/M5Unit-Miniscale
// MIT License
// Modified by Arlo Blythe

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <M5Unified.h>

class Scales {
public:
    Scales();
    ~Scales();

    // SCALES
    int32_t getRawADC();
    float getWeight();
    int32_t getWeightInt();
    String getWeightString();
    float getGapValue();
    void setGapValue(float offset);
    void setOffset(void);
    // RGB LED CTL
    // uint8_t status();
    bool setLEDColor(uint32_t color);
    uint32_t getLEDColor();
    // BUTTON
    uint8_t getBtnStatus();
    uint8_t getFirmwareVersion(void);
    void jumpBootloader(void);
    uint8_t setI2CAddress(uint8_t addr);
    uint8_t getI2CAddress(void);
    bool setLPFilter(uint8_t en);
    uint8_t getLPFilter(void);
    bool setAvgFilter(uint8_t avg);
    uint8_t getAvgFilter(void);
    bool setEmaFilter(uint8_t ema);
    uint8_t getEmaFilter(void);

private:
    uint8_t _addr = DEVICE_DEFAULT_ADDR;
    TwoWire *_wire = &Wire;
    uint8_t _sda = M5.Ex_I2C.getSDA();
    uint8_t _scl = M5.Ex_I2C.getSCL();
    bool writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);

    static const uint8_t DEVICE_DEFAULT_ADDR = 0x26;

    static const uint8_t RAW_ADC_REG = 0x00;
    static const uint8_t CAL_DATA_REG = 0x10;
    static const uint8_t BUTTON_REG = 0x20;
    static const uint8_t RGB_LED_REG = 0x30;
    static const uint8_t SET_GAP_REG = 0x40;
    static const uint8_t SET_OFFESET_REG = 0x50;
    static const uint8_t CAL_DATA_INT_REG = 0x60;
    static const uint8_t CAL_DATA_STRING_REG = 0x70;
    static const uint8_t FILTER_REG = 0x80;
    static const uint8_t JUMP_TO_BOOTLOADER_REG = 0xFD;
    static const uint8_t FIRMWARE_VERSION_REG = 0xFE;
    static const uint8_t I2C_ADDRESS_REG = 0xFF;
};
