#include "vm/parse/ClassFormatParser.h"


void ClassFormatParser::parse(ClassFile &cf, ByteArrayRef ref) {
  parser.reset(ref, 0);
}
