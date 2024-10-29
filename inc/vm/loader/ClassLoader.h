#pragma once

#include <string>

#include "vm/parse/ClassFormatParser.h"

enum CLResultKind {
  CL_FileNotFound,
  CL_FileFormatError,
  CL_Ok
};

struct CLResult {
  CLResultKind kind = CL_Ok;
  CFParserError parse_error;
};

class ClassLoader {

public:
  ClassLoader() = default;

  CLResult load_class_file(const std::string &path);

private:
  ClassFormatParser cfparser;
};
