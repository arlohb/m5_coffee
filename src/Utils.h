#pragma once

#undef B1
#include <fmt/core.h>
#include <M5Unified.h>

template<typename... T>
static void dbg(fmt::format_string<T...> fmt, T&&... args) {
    // Ignores log levels, could be improved
    M5.Log.print(fmt::format(fmt, args...).c_str());
};

template<typename... T>
static void dbgln(fmt::format_string<T...> fmt, T&&... args) {
    // Ignores log levels, could be improved
    M5.Log.println(fmt::format(fmt, args...).c_str());
};

#define dbgval(expr) dbgln(#expr": {}", expr)

static uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    // I really barely understand this format,
    // and don't understand why they do it,
    // but here goes:

    // Its 16 bits, but without alpha
    // 0b rrrr rggg gggb bbbb

    // Reading the src code of DrawPixel,
    // the color is sent as 16 bits,
    // so ignore functions taking in uint32_t color

    // Shrink to 5 bits
    r >>= 3;
    // Shrink to 6 bits
    g >>= 2;
    // Shrink to 5 bits
    b >>= 3;

    return (r << 11) | (g << 5) | b;
}

static uint16_t rgb(uint32_t hex) {
    // Input here is 0x00rrggbb
    // or 0b 0000 0000 rrrr rrrr gggg gggg bbbb bbbb

    uint8_t r = (hex & 0x00FF0000) >> 16;
    uint8_t g = (hex & 0x0000FF00) >> 8;
    uint8_t b = hex & 0x000000FF;

    return rgb(r, g, b);
}

