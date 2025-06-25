#pragma once

#include "State.h"
#include "StatusBar.h"

class LvglState : public State {
public:
    LvglState(const char* titleText, bool padding = true);
    ~LvglState() override;

    State* loop() override;

protected:
    StatusBar statusBar;
    
    lv_obj_t* root;
    lv_obj_t* stateSwitcher;
    
    // Only needs to be in the class as button callbacks need to access it
    State* newState = nullptr;
};
