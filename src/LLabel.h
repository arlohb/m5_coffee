#pragma once

#include "LObject.h"

class LLabel : public LObject {
    public:
        LLabel() = default;
        LLabel(LObject& parent);

        char* GetText();
        void SetText(std::string text);
};

