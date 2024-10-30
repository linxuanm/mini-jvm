#pragma once

#include <string>
#include <fmt/core.h>
#include <fmt/color.h>

#include "core/trace.h"
#include "core/vmdefs.h"
#include "core/error//VMError.h"

#define CHECK_BOUNDS(n_read, ret_val, err_ret_val) \
  do { \
    if (pc + (n_read) >= buf->size()) { \
      err_eof(n_read); return err_ret_val; \
    } \
    return ret_val; \
  } while (0)

struct CodePtrError {
  bool ok       = true;
  pc_t pos      = 0;
  u32 read_size = 0;
  std::string msg;
};

class CodePtr {

public:
  CodePtr() = default;

  [[nodiscard]] bool ok() const { return err.ok; }
  [[nodiscard]] bool more() const { return pc < buf->size(); }
  [[nodiscard]] const CodePtrError &error() const { return err; }
  [[nodiscard]] pc_t get_pc() const { return pc; }
  void reset(pc_t pos) { pc = pos, err = {}; }

  void reset(ByteArrayRef b, pc_t pos) {
    pc = pos, buf = b, err = {}, addr_width = 0;
    for (u32 len = buf->size() - 1; len > 0; addr_width++, len /= 10);
  }

  [[nodiscard]] std::string format_pc(pc_t ptr) const {
    return fmt::format("{:+{}}", static_cast<int>(ptr), addr_width);
  }

  u8 read_u8() { CHECK_BOUNDS(1, _read_u8(), 0); }
  u16 read_u16() { CHECK_BOUNDS(2, _read_u16(), 0); }
  u32 read_u32() { CHECK_BOUNDS(4, _read_u32(), 0); }
  u64 read_u64() { CHECK_BOUNDS(8, _read_u64(), 0); }
  i8 read_i8() { return static_cast<i8>(read_u8()); }
  i16 read_i16() { return static_cast<i16>(read_u16()); }
  i32 read_i32() { return static_cast<i32>(read_u32()); }
  i64 read_i64() { return static_cast<i64>(read_u64()); }
  void skip() { CHECK_BOUNDS(1, void(pc++), void()); }
  void skip_n(u32 n) { CHECK_BOUNDS(n, void(pc += n), void()); }

protected:
  ByteArrayRef buf = nullptr;

private:
  pc_t pc        = 0;
  u32 addr_width = 0;

  CodePtrError err;

  u8 _read_u8() { return (*buf)[pc++]; }
  u16 _read_u16() { return static_cast<u16>(_read_u8()) << 8 | (*buf)[pc++]; }
  u32 _read_u32() { return static_cast<u32>(_read_u16()) << 16 | _read_u16(); }
  u64 _read_u64() { return static_cast<u64>(_read_u32()) << 32 | _read_u32(); }

  void set_err(pc_t pos, u32 read_size, const std::string &msg) {
    if (err.ok || pos < err.pos)
      err = {false, pos, read_size, msg};
  }

  void err_eof(u32 size) { set_err(pc, size, "unexpected EOF"); }
};