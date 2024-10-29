#pragma once

#ifndef ENABLE_TRACE

#include "trace/color.h"
#include "trace/macros.h"
#include "trace/options.h"
#include "trace/printer.h"

#define TRACE_OUT MACRO_TRACE_OUT
#define TRACE_DO MACRO_TRACE_DO

#else

#define TRACE_OUT
#define TRACE_DO

#endif
