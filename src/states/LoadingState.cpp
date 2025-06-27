#include "LoadingState.h"

LoadingState::LoadingState() {}

StateTransition LoadingState::loop() {
    return std::nullopt;
}
