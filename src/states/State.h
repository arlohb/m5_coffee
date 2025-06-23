#pragma once

class State {
public:
    virtual ~State() = default;
    
    virtual void loop() = 0;
};
