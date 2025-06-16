#pragma once

#include "LObject.h"

class LButton : public LObject {
public:
    LButton() = default;
    LButton(LObject& parent);
};

