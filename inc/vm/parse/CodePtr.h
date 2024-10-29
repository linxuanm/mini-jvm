#pragma once

#include <string>

#include "core/trace.h"
#include "core/vmdefs.h"

#define CHECK_BOUNDS(n_read, ret_val, err_ret_val) \
  do { \
    if (pc + (n_read) >= buf->size()) { \
      err_eof(n_read); return err_ret_val; } \
    return ret_val; \
  } while (0)

struct CodePtrError {
  bool ok = false;
  pc_t pos = 0;
  u32 read_size = 0;
  std::string msg;
};

class CodePtr {

public:
  explicit CodePtr(bool trace = false);

  [[nodiscard]] bool ok() const { return error.ok; }
  void reset(pc_t pos) { pc = pos, error = {}; }
  void reset(ByteArrayRef b, pc_t pos) { pc = pos, buf = b, error = {}; }

  u8 read_u8() { CHECK_BOUNDS(1, _read_u8(), 0); }
  u16 read_u16() { CHECK_BOUNDS(2, _read_u16(), 0); }
  u32 read_u32() { CHECK_BOUNDS(4, _read_u32(), 0); }
  u64 read_u64() { CHECK_BOUNDS(8, _read_u64(), 0); }
  i8 read_i8() { return static_cast<i8>(read_u8()); }
  i16 read_i16() { return static_cast<i8>(read_u8()); }
  i32 read_i32() { return static_cast<i8>(read_u8()); }
  i64 read_i64() { return static_cast<i8>(read_u8()); }

private:
  ByteArrayRef buf;
  pc_t pc;

  bool trace_read;
  CodePtrError error;

  u8 _read_u8() {
    const auto val = (*buf)[pc++];
    TRACE_DO(B, { if (trace_read) Trace::O("{:02X} ", val); });
    return val;
  }

  u16 _read_u16() { return static_cast<u16>(_read_u8()) << 8 | (*buf)[pc++]; }
  u32 _read_u32() { return static_cast<u32>(_read_u16()) << 16 | (*buf)[pc++]; }
  u64 _read_u64() { return static_cast<u64>(_read_u32()) << 32 | (*buf)[pc++]; }

  void set_err(pc_t pos, u32 read_size, const std::string &msg) {
    if (error.ok || pos < error.pos)
      error = {false, pos, read_size, msg};
  }

  void err_eof(u32 size) { set_err(pc, size, "unexpected EOF"); }
};