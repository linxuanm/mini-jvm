#pragma once

#include "options.h"

#include <fmt/core.h>

namespace Trace {

template <typename... T>
void O(fmt::format_string<T...> fmt, T &&... args) {
  fmt::print(stdout, fmt, std::forward<T>(args)...);
}

inline void title(const std::string &title) {
  O("{:=^{}s}\n", title, SCREEN_WIDTH);
}

}