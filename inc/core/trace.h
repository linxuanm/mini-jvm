#pragma once

#ifndef ENABLE_TRACE

#include "trace/macros.h"
#define TRACE_OUT MACRO_TRACE_OUT
#define TRACE_DO MACRO_TRACE_DO

#else

#define TRACE_OUT
#define TRACE_DO

#endif