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
