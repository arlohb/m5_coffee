#pragma once

#include "State.h"
#include "StatusBar.h"

class LvglState : public State {
public:
    LvglState(const char* titleText);
    ~LvglState() override;

    State* loop() override;

private:
    StatusBar statusBar;
    
    lv_obj_t* root;
    lv_obj_t* stateSwitcher;
    
    // Only needs to be in the class as button callbacks need to access it
    State* newState = nullptr;
};
