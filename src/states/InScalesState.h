#pragma once

#include "LvglState.h"
#include "../scales/ScalesTask.h"

class InScalesState : public LvglState {
public:
    InScalesState(const std::string& selectedCoffee);
    ~InScalesState() override = default;

    StateTransition loop() override;

private:
    std::string selectedCoffee;
    
    ScalesTask scales;
    
    lv_obj_t* weightLabel;
};
