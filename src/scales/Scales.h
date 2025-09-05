#pragma once

class Scales {
public:
    virtual ~Scales() = default;
    
    virtual float getWeight() = 0;
    virtual void zero() = 0;
};
