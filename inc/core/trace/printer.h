#pragma once

#include "options.h"
#include "fmt/xchar.h"

#include <iomanip>
#include <sstream>
#include <fmt/core.h>

namespace Trace {

enum Align { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };

// global line buffer for more control over output
extern std::stringstream ss;

std::string fit_in_width(const std::string &s, int width,
                         const std::string &pad, Align align = ALIGN_LEFT);

template <typename... T>
void O(fmt::format_string<T...> fmt, T &&... args) {
  fmt::print(stdout, fmt, std::forward<T>(args)...);
}

template <typename... T>
void OS(fmt::format_string<T...> fmt, T &&... args) {
  ss << fmt::format(fmt, std::forward<T>(args)...);
}

static void bar_with_bounds(const std::string &l, const std::string &r) {
  std::stringstream ss;
  for (auto i = 0; i < SCREEN_WIDTH - 2; i++)
    ss << "─";
  O("{}{}{}\n", l, ss.str(), r);
}

static std::string col_2_block(const std::string &s, Align align) {
  return fit_in_width(s, COL_WIDTH_2, " ", align);
}

inline void center_text(const std::string &s) {
  O("│ {} │\n", fit_in_width(s, COL_WIDTH_1, " ", ALIGN_CENTER));
}

inline void top_bar() { bar_with_bounds("╭", "╮"); }
inline void bottom_bar() { bar_with_bounds("╰", "╯"); }
inline void mid_bar() { bar_with_bounds("├", "┤"); }

inline void title(const std::string &s) {
  top_bar();
  center_text(s);
  mid_bar();
}

void mid_bar_text(const std::string &s);

inline void col_2(const std::string &a, const std::string &b,
                  Align aa = ALIGN_LEFT, Align ab = ALIGN_LEFT) {
  O("│ {}  {} │\n", col_2_block(a, aa), col_2_block(b, ab));
}
}