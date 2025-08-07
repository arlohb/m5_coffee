#pragma once

#include "LvglState.h"

class MenuState : public LvglState {
public:
    MenuState();
    
    std::optional<StateTransition> loop() override;
};
