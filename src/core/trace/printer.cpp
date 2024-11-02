#include "core/trace/printer.h"

#include <regex>

static std::string strip_ansi_codes(const std::string &input) {
  return std::regex_replace(input, std::regex("\x1B\\[.*?m"), "");
}

std::string Trace::fit_in_width(const std::string &s, int width,
                                const std::string &pad, Align align) {
  const auto plain   = strip_ansi_codes(s);
  const bool shorten = plain.length() > width;
  const auto len     = shorten ? width - 4 : plain.length();
  std::stringstream ss;
  for (auto si = 0, pi = 0; pi < len; si++, pi++) {
    for (; plain[pi] != s[si]; si++)
      ss << s[si];
    ss << plain[pi];
  }
  if (shorten)
    ss << " ...";
  const auto seq = ss.str();
  ss.str("");
  ss.clear();
  const auto pad_size = shorten ? 0 : width - plain.length();
  switch (align) {
  case ALIGN_RIGHT:
    for (auto i = 0; i < pad_size; i++)
      ss << pad;
    ss << seq;
    break;
  case ALIGN_LEFT:
    ss << seq;
    for (auto i = 0; i < pad_size; i++)
      ss << pad;
    break;
  case ALIGN_CENTER: {
    for (auto i = 0; i < pad_size >> 1; i++)
      ss << pad;
    ss << seq;
    for (auto i = 0; i < pad_size - (pad_size >> 1); i++)
      ss << pad;
    break;
  }
  }
  return ss.str();
}

void Trace::mid_bar_text(const std::string &s) {
  const auto padded = fmt::format(" {} ", s);
  O("├─{}─┤\n", fit_in_width(padded, COL_WIDTH_1, "─", ALIGN_CENTER));
}