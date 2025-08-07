#pragma once

#include <mutex>
#include "State.h"
#include "StatusBar.h"

class LvglState : public State {
public:
    static const int32_t PADDING = 16;

    LvglState(const char* titleText, bool padding = true);
    ~LvglState() override;

    std::optional<StateTransition> loop() override;

protected:
    StatusBar statusBar;
    
    lv_obj_t* root;

    /// Mutex for the state transition.
    /// Only needs to be used when setting stateTransition from a different thread,
    std::mutex stateTransitionMutex;
    std::optional<StateTransition> stateTransition = std::nullopt;
};
