#pragma once

#include "ByteParser.h"
#include "ClassFile.h"

enum CFErrorKind {
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
};