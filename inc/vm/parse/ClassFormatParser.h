#pragma once

#include "ByteParser.h"
#include "ClassFile.h"

typedef std::pair<const char *, u32> TraceEntry;

enum CFErrorKind {
  CF_NotAClassFile,
  CF_VersionMismatch,
  // Any {CodePtrError}
  CF_MalformedBytecode,
  // UTF-8 encoding problem
  CF_MalformedUTF8,
  CF_InvalidIndex,
  CF_InvalidFlag
};

struct CFParserError {
  bool ok  = true;
  pc_t pos = 0;
  CFErrorKind err_kind;
  std::string err_msg;
};

class ClassFormatParser {

public:
  ClassFormatParser() = default;
  [[nodiscard]] bool ok() const { return err.ok; }
  [[nodiscard]] const CFParserError &error() const { return err; }
  void parse(ClassFile &cf, ByteArrayRef ref);

private:
  ByteParser parser;
  CFParserError err;

  void traces(pc_t pos, const std::vector<TraceEntry> &entries) const;
  void parse_constant_pool_entry(ClassFile &cf, u16 &i);
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

  u16 read_cp_index(const ClassFile &cf, const char *tag, bool zero = false) {
    const u16 index = parser.read_wtag_u16(tag);
    if (!parser.ok()) return 0;
    if ((index == 0 && !zero) || index >= cf.constant_pool.size()) {
      err_atpc(CF_InvalidIndex,
               "constant pool index must be in range [1, {}], got {}",
               cf.constant_pool.size() - 1, index);
      return 0;
    }
    return index;
  }

};