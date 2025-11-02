#pragma once

#include "Scales.h"
#include <HX711.h>
#include <mutex>

class HX711Scales : public Scales {
public:
    HX711Scales();
    ~HX711Scales() override;

    void zero() override;
    float getWeight() override;

private:
    static const bool calibrateOnStart = false;
    static constexpr float scaleFactor = 878.0739f;
    static const int DOUT_PIN = 33;
    static const int SCK_PIN  = 32;
    
    std::mutex mutex;

    HX711 scales;

    void calibrate();
};
