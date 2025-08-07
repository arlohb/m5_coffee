#pragma once

#include <functional>
#include <optional>

class State;

using StateTransition = std::function<State*()>;

class State {
public:
    virtual ~State() = default;
    
    virtual std::optional<StateTransition> loop() = 0;
};
