#pragma once

#include "ByteParser.h"
#include "ClassFile.h"

typedef std::pair<const char *, u32> TraceEntry;

enum CFErrorKind {
  CF_None,
  CF_NotAClassFile,
  CF_VersionMismatch,
  // Any {CodePtrError}
  CF_MalformedBytecode,
  // UTF-8 encoding problem
  CF_MalformedUTF8,
  CF_InvalidIndex,
  CF_MismatchedCPType,
  CF_InvalidFlag
};

struct CFParserError {
  bool ok              = true;
  pc_t pos             = 0;
  CFErrorKind err_kind = CF_None;
  std::string err_msg;
};

class ClassFormatParser {

public:
  ClassFormatParser(): cf{nullptr} {};
  [[nodiscard]] bool ok() const { return err.ok; }
  [[nodiscard]] const CFParserError &error() const { return err; }
  void parse(ClassFile *in_cf, ByteArrayRef ref);

private:
  ClassFile *cf;
  ByteParser parser;
  CFParserError err;

  void traces(pc_t pos, const std::vector<TraceEntry> &entries) const;
  void parse_constant_pool_entry(u16 &i);
  void verify_constant_pool_entry(u16 &i);
  bool verify_cp_index(u16 index, ConstantPoolTag tag);
  void parse_field_entry(u16 &i);
  std::string decode_modified_utf8(u16 length);

  template <typename... T>
  void err_atpc(CFErrorKind kind, fmt::format_string<T...> f, T &&... args) {
    if (err.ok || parser.get_pc() < err.pos)
      err = {
          false, parser.get_pc(), kind,
          fmt::format(f, std::forward<T>(args)...)
      };
  }

  bool parser_ok() {
    const bool no_err = parser.ok();
    if (!no_err && err.ok) {
      auto [ok, pos, read_size, msg] = parser.error();
      err_atpc(CF_MalformedBytecode, "{} (reading {} bytes)", msg, read_size);
    }
    return no_err;
  }

  u16 read_cp_index(const char *msg, ConstantPoolTag tag) {
    const u16 index = parser.read_wtag_u16(msg);
    if (!parser.ok()) return 0;
    if (index == 0 || index >= cf->constant_pool.size()) {
      err_atpc(CF_InvalidIndex,
               "constant pool index must be in range [1, {}], got {}",
               cf->constant_pool.size() - 1, index);
      return 0;
    }
    return index;
  }
};