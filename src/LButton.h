#pragma once

#include <optional>
#include "LObject.h"
#include "LLabel.h"

class LButton : public LObject {
public:
    LButton() = default;
    LButton(LObject& parent);
};

