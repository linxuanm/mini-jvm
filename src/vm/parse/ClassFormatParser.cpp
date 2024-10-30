#include "vm/parse/ClassFormatParser.h"

void ClassFormatParser::traces(pc_t pos,
                               const std::vector<TraceEntry> &entries) const {
  for (auto [tag, size] : entries) {
    parser.trace_bytes(tag, pos, size);
    pos += size;
  }
}

void ClassFormatParser::parse(ClassFile &cf, ByteArrayRef ref) {
  parser.reset(ref, 0);

  const u32 magic = parser.read_wtag_u32("magic");
  if (!parser_ok()) return;
  if (magic != 0xCAFEBABE)
    return err_atpc(CF_NotAClassFile, "invalid header 0x{:X}", magic);

  parser.read_wtag_u16("minor version");
  const u16 major = parser.read_wtag_u16("major version");
  if (!parser_ok()) return;
  if (major != 65)
    return err_atpc(CF_VersionMismatch, "require version 65, got {}", major);

  TRACE_DO(B, { Trace::mid_bar_text("Constant Pool"); });
  cf.constant_pool.reserve(parser.read_wtag_u16("constant pool count"));
  if (!parser_ok()) return;
}