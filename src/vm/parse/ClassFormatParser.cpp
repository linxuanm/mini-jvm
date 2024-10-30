#include "vm/parse/ClassFormatParser.h"

static const std::vector<TraceEntry> TRACE_HEADER = {
  {"magic number", 4},
  {"minor version", 2},
  {"major version", 2}
};

void ClassFormatParser::traces(pc_t pos,
                               const std::vector<TraceEntry> &entries) const {
  for (auto [tag, size] : entries) {
    parser.trace_bytes(tag, pos, size);
    pos += size;
  }
}

void ClassFormatParser::parse(ClassFile &cf, ByteArrayRef ref) {
  parser.reset(ref, 0);
  pc_t pos = 0;

  traces(0, TRACE_HEADER);
}