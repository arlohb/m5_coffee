#include "HX711Scales.h"

#include "../Utils.h"

HX711Scales::HX711Scales() {
    scales.begin(DOUT_PIN, SCK_PIN);
    scales.set_gain();
    scales.set_scale(scaleFactor);
    scales.tare();
    
    if (calibrateOnStart) calibrate();
}

HX711Scales::~HX711Scales() {}

void HX711Scales::zero() {
    mutex.lock();
    scales.tare(10);
    mutex.unlock();
}

float HX711Scales::getWeight() {
    mutex.lock();
    float weight = scales.get_units(5);
    mutex.unlock();
    return weight;
}

void HX711Scales::calibrate() {
    const float calibrationWeight = 203.0f;

    scales.tare(30);

    LOG_INFO("Place {}g weight on scales", calibrationWeight);
    delay(5000);
    long rawAdc0 = scales.read_average(30);
    rawAdc0 -= scales.get_offset();
    
    float scale = (float)rawAdc0 / calibrationWeight;
    scales.set_scale(scale);
    LOG_INFO("Calibration complete, scale set to {}", scale);

}
