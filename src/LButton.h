#pragma once

#include <optional>
#include "LObject.h"
#include "LLabel.h"

class LButton : public LObject {
public:
    LButton() = default;
    LButton(LObject& parent);
    
    void setLabel(const std::string& text);
    
public:
    std::optional<LLabel> label;
};

