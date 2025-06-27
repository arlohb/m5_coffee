#pragma once

#undef B1
#include <fmt/core.h>
#include <M5Unified.h>

namespace __detail {
    std::string getMethodName(const char* prettyFunction);
}

#define LOG_INFO(fmt_str, ...) { \
    std::string tag = __detail::getMethodName(__PRETTY_FUNCTION__); \
\
    std::string str = fmt::format(fmt_str, ##__VA_ARGS__); \
    ESP_LOGI(tag.c_str(), "%s", str.c_str()); \
}
