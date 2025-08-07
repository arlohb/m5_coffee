#pragma once

#include "LvglState.h"

class GrindState : public LvglState {
public:
    GrindState(const std::string& selectedCoffee, float weight);

private:
    std::string selectedCoffee;
    float weight;
    int grindSize = 40;
    lv_obj_t* grindLabel;
    
    void updateGrindLabel();
};
