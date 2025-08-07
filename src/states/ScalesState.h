#pragma once

#include "LvglState.h"
#include "../scales/ScalesTask.h"

class ScalesState : public LvglState {
public:
    ScalesState(std::function<StateTransition(float weight)> next);
    ~ScalesState() override = default;

    std::optional<StateTransition> loop() override;

private:
    std::function<StateTransition(float weight)> nextStateTransition;

    ScalesTask scales;
    
    lv_obj_t* weightLabel;
};
