#include "parse/CodePtr.h"

CodePtr::CodePtr(bool trace): buf{nullptr}, pc{0}, trace_read{trace} {
}