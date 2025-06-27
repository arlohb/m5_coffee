// https://github.com/m5stack/M5Unit-Miniscale
// MIT License
// Modified by Arlo Blythe

#include "Scales.h"
#include "../Utils.h"

Scales::Scales() {
    _wire->begin(_sda, _scl, 400000UL);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error) {
        dbgln("Scales not found at address 0x{:02X}, error code: {}", _addr, error);
    }

    dbgln("Scales loaded : firmware version: v{}", getFirmwareVersion());
    
    setLEDColor(0x000000);
    setOffset();
    
    if (calibrateOnStart) calibrate();
}

Scales::~Scales() {
    if (!_wire->end()) {
        dbgln("Error ending I2C communication");
    } else {
        dbgln("Scales I2C communication ended successfully");
    }
}

bool Scales::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                             uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->write(buffer, length);
    if (_wire->endTransmission() == 0) return true;
    return false;
}

bool Scales::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                            uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

uint8_t Scales::getBtnStatus() {
    uint8_t data = 0;
    readBytes(_addr, BUTTON_REG, &data, 1);
    return data;
}

bool Scales::setLEDColor(uint32_t colorHEX) {
    uint8_t color[4] = {0};
    // RED
    color[0] = (colorHEX >> 16) & 0xff;
    // GREEN
    color[1] = (colorHEX >> 8) & 0xff;
    // BLUE
    color[2] = colorHEX & 0xff;
    return writeBytes(_addr, RGB_LED_REG, color, 3);
}

bool Scales::setLPFilter(uint8_t en) {
    uint8_t reg = FILTER_REG;

    return writeBytes(_addr, reg, (uint8_t *)&en, 1);
}

uint8_t Scales::getLPFilter(void) {
    uint8_t data;
    uint8_t reg = FILTER_REG;

    readBytes(_addr, reg, (uint8_t *)&data, 1);

    return data;
}

bool Scales::setAvgFilter(uint8_t avg) {
    uint8_t reg = FILTER_REG + 1;

    return writeBytes(_addr, reg, (uint8_t *)&avg, 1);
}

uint8_t Scales::getAvgFilter(void) {
    uint8_t data;
    uint8_t reg = FILTER_REG + 1;

    readBytes(_addr, reg, (uint8_t *)&data, 1);

    return data;
}

bool Scales::setEmaFilter(uint8_t ema) {
    uint8_t reg = FILTER_REG + 2;

    return writeBytes(_addr, reg, (uint8_t *)&ema, 1);
}

uint8_t Scales::getEmaFilter(void) {
    uint8_t data;
    uint8_t reg = FILTER_REG + 2;

    readBytes(_addr, reg, (uint8_t *)&data, 1);

    return data;
}

uint32_t Scales::getLEDColor() {
    uint8_t color[4]  = {0};
    uint32_t colorHEX = 0;
    if (readBytes(_addr, RGB_LED_REG, color, 3)) {
        colorHEX = color[0];
        colorHEX = (colorHEX << 8) | color[1];
        colorHEX = (colorHEX << 8) | color[2];
    };
    return colorHEX;
}

float Scales::getWeight() {
    uint8_t data[4];
    float c;
    uint8_t *p;

    if (readBytes(_addr, CAL_DATA_REG, data, 4)) {
        p = (uint8_t *)&c;
        memcpy(p, data, 4);
    };
    return c;
}

int32_t Scales::getWeightInt() {
    uint8_t data[4];

    readBytes(_addr, CAL_DATA_INT_REG, data, 4);
    return (data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
}

String Scales::getWeightString() {
    char *p;
    uint8_t data[16];
    String res;

    readBytes(_addr, CAL_DATA_STRING_REG, data, 16);
    p   = (char *)data;
    res = p;
    return res;
}

float Scales::getGapValue() {
    uint8_t data[4];
    float c;
    uint8_t *p;

    if (readBytes(_addr, SET_GAP_REG, data, 4)) {
        p = (uint8_t *)&c;
        memcpy(p, data, 4);
    };
    return c;
}

void Scales::setGapValue(float offset) {
    uint8_t datatmp[4];
    uint8_t *p;
    p = (uint8_t *)&offset;

    memcpy(datatmp, p, 4);

    writeBytes(_addr, SET_GAP_REG, datatmp, 4);
    delay(100);
}

void Scales::setOffset(void) {
    uint8_t datatmp[4];
    datatmp[0] = 1;

    writeBytes(_addr, SET_OFFESET_REG, datatmp, 1);
}

int32_t Scales::getRawADC() {
    uint8_t data[4] = {0};
    int rawADC      = 0;
    if (readBytes(_addr, RAW_ADC_REG, data, 4)) {
        rawADC = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
    };

    return rawADC;
}

uint8_t Scales::setI2CAddress(uint8_t addr) {
    _wire->beginTransmission(_addr);
    _wire->write(I2C_ADDRESS_REG);
    _wire->write(addr);
    _wire->endTransmission();
    _addr = addr;
    return _addr;
}

uint8_t Scales::getI2CAddress(void) {
    _wire->beginTransmission(_addr);
    _wire->write(I2C_ADDRESS_REG);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, (size_t)1);
    RegValue = Wire.read();
    return RegValue;
}

uint8_t Scales::getFirmwareVersion(void) {
    _wire->beginTransmission(_addr);
    _wire->write(FIRMWARE_VERSION_REG);
    _wire->endTransmission();

    uint8_t RegValue;

    _wire->requestFrom(_addr, (size_t)1);
    RegValue = Wire.read();
    return RegValue;
}

void Scales::jumpBootloader(void) {
    uint8_t value = 1;

    writeBytes(_addr, JUMP_TO_BOOTLOADER_REG, (uint8_t *)&value, 1);
}

void Scales::calibrate() {
    const float originalGapValue = getGapValue();

    dbgln("Calibrating scales...");
    
    setOffset();
    delay(1000);
    int32_t rawAdc0 = getRawADC();
    
    dbgln("Place {}g weight on scales", calibrationWeight);
    delay(5000);
    int32_t rawAdc1 = getRawADC();
    
    dbgln("Raw ADC values: {} (0g), {} ({}g)", rawAdc0, rawAdc1, calibrationWeight);
    
    float gapValue = (float)(std::abs(rawAdc1 - rawAdc0)) / calibrationWeight;
    setGapValue(gapValue);
    
    dbgln("Calibration complete, gap value set to {} (was {})", gapValue, originalGapValue);
}