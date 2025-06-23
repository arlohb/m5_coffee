#pragma once

#include "State.h"

class LoadingState : public State {
public:
    LoadingState();

    State* loop() override;
};
