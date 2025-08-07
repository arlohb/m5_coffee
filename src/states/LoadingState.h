#pragma once

#include "State.h"

class LoadingState : public State {
public:
    LoadingState();

    std::optional<StateTransition> loop() override;
};
