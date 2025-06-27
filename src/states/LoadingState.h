#pragma once

#include "State.h"

class LoadingState : public State {
public:
    LoadingState();

    StateTransition loop() override;
};
