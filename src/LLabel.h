#pragma once

#include "LObject.h"

class LLabel : public LObject {
    public:
        LLabel() = default;
        LLabel(LObject& parent);

        void SetText(std::string text);
};

