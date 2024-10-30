#include "vm/parse/ClassFormatParser.h"

static const std::vector<TraceEntry> T_HEADER = {
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

  u32 magic = parser.read_u32();
  if (!parser_ok())
    return;
  if (magic != 0xCAFEBABE)
    return err_atpc(CF_NotAClassFile, "invalid header 0x{:X}", magic);

  u16 minor = parser.read_u16();
  u16 major = parser.read_u16();
  if (!parser_ok())
    return;
  if (major != 65)
    return err_atpc(CF_VersionMismatch,
                    "only class format 65 is supported, got {}", major);

  TRACE_DO(B, { traces(pos, T_HEADER); Trace::mid_bar_text("Constant Pool"); });
}