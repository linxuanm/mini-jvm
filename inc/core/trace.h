#pragma once

#ifndef ENABLE_TRACE

#include "core/display/color.h"
#include "core/trace/macros.h"
#include "core/trace/options.h"
#include "core/trace/printer.h"

#define TRACE_OUT MACRO_TRACE_OUT
#define TRACE_DO MACRO_TRACE_DO

#else

#define TRACE_OUT
#define TRACE_DO

#endif
