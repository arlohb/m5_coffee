#pragma once

#include "State.h"

class LoadingState : public State {
public:
    LoadingState();

    void loop() override;
};
