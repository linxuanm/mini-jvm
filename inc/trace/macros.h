#pragma once

#include <fmt/core.h>

#include "options.h"

#define TRACE_OUT(opt, format, ...) \
  do { if (Trace::opt()) fmt::print(format, ##__VA_ARGS__); } while (0)

#define TRACE_DO(opt, stmts) \
  do { if (Trace::opt()) { stmts } } while (0)
