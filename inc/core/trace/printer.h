#pragma once

#include "options.h"
#include "fmt/xchar.h"

#include <sstream>
#include <fmt/core.h>

namespace Trace {

enum Align { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };

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

inline std::string trunc_to_width(const std::string &s, int size) {
  return s.length() <= size ? s : s.substr(0, size - 4) + " ...";
}

namespace Comp {
inline void bar_with_bounds(const std::string &l, const std::string &r) {
  std::stringstream ss;
  for (auto i = 0; i < SCREEN_WIDTH - 2; i++)
    ss << "─";
  O("{}{}{}\n", l, ss.str(), r);
}

inline std::string col_2_block(const std::string &s, Align align) {
  auto trunc = trunc_to_width(s, COL_WIDTH_2);
  switch (align) {
  case ALIGN_LEFT:
    return fmt::format("{:<{}}", trunc, COL_WIDTH_2);
  case ALIGN_RIGHT:
    return fmt::format("{:>{}}", trunc, COL_WIDTH_2);
  case ALIGN_CENTER:
    return fmt::format("{:^{}}", trunc, COL_WIDTH_2);
  }
  return "";
}
}

inline void center_text(const std::string &s) {
  O("│ {:^{}} │\n", trunc_to_width(s, COL_WIDTH_1), COL_WIDTH_1);
}

inline void top_bar() { Comp::bar_with_bounds("╭", "╮"); }
inline void bottom_bar() { Comp::bar_with_bounds("╰", "╯"); }
inline void mid_bar() { Comp::bar_with_bounds("├", "┤"); }

inline void mid_bar_text(const std::string &s) {
  const auto padded = fmt::format(" {} ", trunc_to_width(s, COL_WIDTH_1));
  const auto total  = SCREEN_WIDTH - 2 - padded.length();
  const auto left   = total >> 1;
  const auto right  = total - left;

  std::stringstream ss;
  ss << "├";
  for (auto i = 0; i < left; i++)
    ss << "─";
  ss << padded;
  for (auto i = 0; i < right; i++)
    ss << "─";
  ss << "┤";
  O("{}\n", ss.str());
}

inline void col_2(const std::string &a, const std::string &b,
                  Align aa = ALIGN_LEFT, Align ab = ALIGN_LEFT) {
  O("│ {}  {} │\n", Comp::col_2_block(a, aa), Comp::col_2_block(b, ab));
}
}
