#pragma once

#include "LvglState.h"
#include "../scales/ScalesTask.h"

class InScalesState : public LvglState {
public:
    InScalesState(std::function<StateTransition(float weight)> next);
    ~InScalesState() override = default;

    std::optional<StateTransition> loop() override;

private:
    std::function<StateTransition(float weight)> nextStateTransition;

    ScalesTask scales;
    
    lv_obj_t* weightLabel;
};
