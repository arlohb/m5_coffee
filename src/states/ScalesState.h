#pragma once

#include <atomic>
#include "LvglState.h"
#include "../scales/ScalesTask.h"

class ScalesState : public LvglState {
public:
    ScalesState();
    ~ScalesState() override;

    State* loop() override;

private:
    ScalesTask scales;
    
    lv_obj_t* weightLabel;
};
