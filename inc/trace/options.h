#pragma once

#include <fmt/core.h>

#include "color.h"

#define GEN_TRACE_FUNC_FOR(short, opt_entry) \
  inline bool short() { return OPTIONS.opt_entry; }

namespace Trace {

struct TraceOptions {
  bool runtime;
  bool binparse;
  bool validation;
  bool instantiation;
  bool execution;
  bool jit;
};

constexpr int SCREEN_WIDTH = 80;
extern TraceOptions OPTIONS;

inline void configure(const TraceOptions &opt) {
  if (opt.runtime) {
    auto ON = fmt::format("{}✅ {}", Color::GREEN, Color::UNDERLINE);
    auto OFF = fmt::format("{}❌ ", Color::RED);
    
    fmt::print("{}Tracing Configuration:\n{}", Color::BOLD, Color::DEFAULT);
    fmt::print("{}Runtime\n", ON);
    fmt::print("{}Binary Parsing\n", opt.binparse ? ON : OFF);
    fmt::print("{}Validation\n", opt.validation ? ON : OFF);
    fmt::print("{}Instantiation\n", opt.instantiation ? ON : OFF);
    fmt::print("{}Execution\n", opt.execution ? ON : OFF);
    fmt::print("{}JIT\n", opt.jit ? ON : OFF);
  }

  OPTIONS = opt;
}

GEN_TRACE_FUNC_FOR(R, runtime);
GEN_TRACE_FUNC_FOR(B, binparse);
GEN_TRACE_FUNC_FOR(V, validation);
GEN_TRACE_FUNC_FOR(I, instantiation);
GEN_TRACE_FUNC_FOR(E, execution);
GEN_TRACE_FUNC_FOR(J, jit);

}