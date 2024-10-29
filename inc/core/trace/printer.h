#pragma once

#include "options.h"
#include "fmt/xchar.h"

#include <sstream>
#include <fmt/core.h>

namespace Trace {
// global line buffer for more control over output
extern std::stringstream ss;

template <typename... T>
void O(fmt::format_string<T...> fmt, T &&... args) {
  fmt::print(stdout, fmt, std::forward<T>(args)...);
}

template <typename... T>
void OS(fmt::format_string<T...> fmt, T &&... args) {
  ss << fmt::format(fmt, std::forward<T>(args)...);
}

inline void CLEAR_LINEBUF() { ss.clear(); }

namespace Components {
inline void bar_with_bounds(const std::string &l, const std::string &r) {
  std::stringstream ss;
  for (auto i = 0; i < COL_WIDTH_1 - 2; i++) ss << "─";
  O("{}{}{}", l, ss.str(), r);
}
}

inline std::string trunc_to_width(const std::string &s, int size) {
  return s.length() <= size ? s : s.substr(0, size - 4) + " ...";
}

inline void top_bar() { Components::bar_with_bounds("╭", "╮"); }
inline void bottom_bar() { Components::bar_with_bounds("╰", "╯"); }
inline void mid_bar() { Components::bar_with_bounds("├", "┤"); }
inline void mid_bar_text(const std::string &s) {
  auto padded = fmt::format(" {} ", trunc_to_width(s, COL_WIDTH_1));
  O("{}{:-^{}}{}", "├", padded, SCREEN_WIDTH - 2, "┤");
}
}