#pragma once

#include "ByteParser.h"
#include "ClassFile.h"

typedef std::pair<const char *, u32> TraceEntry;

enum CFErrorKind {
  CF_NotAClassFile,
  CF_VersionMismatch,
  // Any {CodePtrError}
  CF_MalformedBytecode,
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
  [[nodiscard]] const CFParserError &error() const { return err; };
  void parse(ClassFile &cf, ByteArrayRef ref);

private:
  ByteParser parser;
  CFParserError err;

  void traces(pc_t pos, const std::vector<TraceEntry> &entries) const;
  void parse_constant_pool_entry(const ClassFile &cf, u16 i);

  template <typename... T>
  void err_atpc(CFErrorKind kind, fmt::format_string<T...> f, T &&... args) {
    if (err.ok || parser.get_pc() < err.pos)
      err = {
        false, parser.get_pc(), kind,
        fmt::format(f, std::forward<T>(args)...)
      };
  }

  bool parser_ok() {
    const bool ok = parser.ok();
    if (!ok) {
      auto [ok, pos, read_size, msg] = parser.error();
      err_atpc(CF_MalformedBytecode, "{} (reading {} bytes)", msg, read_size);
    }
    return ok;
  }
};