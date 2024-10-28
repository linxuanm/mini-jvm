#pragma once

#include <fmt/core.h>

#include "options.h"

#define MACRO_TRACE_OUT(opt, format, ...) \
  do { if (Trace::opt()) fmt::print(format, ##__VA_ARGS__); } while (0)

#define MACRO_TRACE_DO(opt, stmts) \
  do { if (Trace::opt()) { stmts } } while (0)
