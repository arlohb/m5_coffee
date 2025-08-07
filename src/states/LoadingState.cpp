#include "LoadingState.h"

LoadingState::LoadingState() {}

std::optional<StateTransition> LoadingState::loop() {
    return std::nullopt;
}
