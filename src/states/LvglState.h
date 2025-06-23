#pragma once

#include "State.h"
#include "StatusBar.h"

class LvglState : public State {
public:
    LvglState(const char* titleText);
    ~LvglState() override;

    void loop() override;

private:
    StatusBar statusBar;
    
    lv_obj_t* root;
};
