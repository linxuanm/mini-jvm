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

  void traces(pc_t pos, const std::vector<TraceEntry>& entries) const;

  void err_atpc(CFErrorKind kind, const std::string &msg) {
    if (err.ok || parser.get_pc() < err.pos)
      err = {false, parser.get_pc(), kind, msg};
  }

  bool parser_ok() {
    const bool ok = parser.ok();
    if (!ok) {
      auto [ok, pos, read_size, msg] = parser.error();
      const auto detail = fmt::format("{} (reading {} bytes)", msg, read_size);
      err_atpc(CF_MalformedBytecode, detail);
    }
    return ok;
  }
};