#include "Program.h"

extern "C" void app_main() {
    initArduino();

    // Colour buffer is too big for the stack
    Program* program = new Program();

    while (true) {
        program->Loop();
    }
}

