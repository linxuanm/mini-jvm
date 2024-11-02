#pragma once

#include "core/trace.h"
#include "vm/parse/CodePtr.h"

class ByteParser : public CodePtr {

public:
  void trace_bytes(const std::string &tag, u32 n) const;
  void trace_bytes(const std::string &tag, pc_t pos, u32 n) const;

  u8 read_wtag_u8(const char *tag) {
    TRACE_DO(B, { trace_bytes(tag, 1); });
    return read_u8();
  }

  u16 read_wtag_u16(const char *tag) {
    TRACE_DO(B, { trace_bytes(tag, 2); });
    return read_u16();
  }

  u32 read_wtag_u32(const char *tag) {
    TRACE_DO(B, { trace_bytes(tag, 4); });
    return read_u32();
  }

  u64 read_wtag_u64(const char *tag) {
    TRACE_DO(B, { trace_bytes(tag, 8); });
    return read_u64();
  }

private:

};