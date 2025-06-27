#pragma once

#include <functional>
#include <optional>

class State;

using StateTransition = std::optional<std::function<State*()>>;

class State {
public:
    virtual ~State() = default;
    
    virtual StateTransition loop() = 0;
};
