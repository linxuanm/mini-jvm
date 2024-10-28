#pragma once

#define GEN_TRACE_FUNC_FOR(short, opt_entry) \
  inline bool short() { return OPTIONS.opt_entry; }

namespace Trace {

struct TraceOptions {
  bool binparse;
  bool validation;
  bool instantiation;
  bool execution;
  bool jit;
};

constexpr int SCREEN_WIDTH = 80;
extern TraceOptions OPTIONS;

inline void configure(const TraceOptions &options) { OPTIONS = options; }

GEN_TRACE_FUNC_FOR(B, binparse);
GEN_TRACE_FUNC_FOR(V, binparse);
GEN_TRACE_FUNC_FOR(I, binparse);
GEN_TRACE_FUNC_FOR(E, binparse);
GEN_TRACE_FUNC_FOR(J, binparse);

}

namespace Color {

constexpr auto BLACK = "\x1B[0;30m";
constexpr auto RED = "\x1B[0;31m";
constexpr auto GREEN = "\x1B[0;32m";
constexpr auto BROWN = "\x1B[0;33m";
constexpr auto BLUE = "\x1B[0;34m";
constexpr auto PURPLE = "\x1B[0;35m";
constexpr auto CYAN = "\x1B[0;36m";
constexpr auto LIGHTGRAY = "\x1B[0;37m";
constexpr auto DARKGRAY = "\x1B[1;30m";
constexpr auto BRIGHTRED = "\x1B[1;31m";
constexpr auto BRIGHTGREEN = "\x1B[1;32m";
constexpr auto YELLOW = "\x1B[1;33m";
constexpr auto BRIGHTBLUE = "\x1B[1;34m";
constexpr auto MAGENTA = "\x1B[1;35m";
constexpr auto BRIGHTCYAN = "\x1B[1;36m";
constexpr auto WHITE = "\x1B[1;37m";
constexpr auto DEFAULT = "\x1B[0m";

constexpr auto UNDERLINE = "\x1B[4m";
constexpr auto BOLD = "\x1B[1m";

}