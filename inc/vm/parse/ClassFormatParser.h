#pragma once

#include "ByteParser.h"
#include "ClassFile.h"

class ClassFormatParser {

public:
  ClassFormatParser() = default;
  void reset(ByteArrayRef ref) { parser.reset(ref, 0); }
  void parse(ClassFile &cf);

private:
  ByteParser parser;
};
