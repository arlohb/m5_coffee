#pragma once

class State {
public:
    virtual ~State() = default;
    
    virtual State* loop() = 0;
};
