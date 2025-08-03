#pragma once

#include "LvglState.h"

class MenuState : public LvglState {
public:
    MenuState();
    
    StateTransition loop() override;
};
