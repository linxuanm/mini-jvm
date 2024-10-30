#include "vm/parse/ClassFormatParser.h"

#include "core/specs/class_flags.h"

void ClassFormatParser::traces(pc_t pos,
                               const std::vector<TraceEntry> &entries) const {
  for (auto [tag, size] : entries) {
    parser.trace_bytes(tag, pos, size);
    pos += size;
  }
}

void ClassFormatParser::parse_constant_pool_entry(const ClassFile &cf, u16 i) {
  const pc_t pos = parser.get_pc();
  const auto tag = static_cast<ConstantPoolTag>(parser.read_u8());
  if (!parser_ok()) return;
  TRACE_DO(B, { parser.trace_bytes(render_CONSTANT(tag), pos, 1); });

  switch (tag) {
  case CONSTANT_Utf8: break;
  case CONSTANT_Integer: break;
  case CONSTANT_Float: break;
  case CONSTANT_Long: break;
  case CONSTANT_Double: break;
  case CONSTANT_Class: break;
  case CONSTANT_String: break;
  case CONSTANT_Fieldref: break;
  case CONSTANT_Methodref: break;
  case CONSTANT_InterfaceMethodref: break;
  case CONSTANT_NameAndType: break;
  case CONSTANT_MethodHandle: break;
  case CONSTANT_MethodType: break;
  case CONSTANT_Dynamic: break;
  case CONSTANT_InvokeDynamic: break;
  case CONSTANT_Module: break;
  case CONSTANT_Package: break;
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
  cf.constant_pool.resize(parser.read_wtag_u16("constant pool count"));
  if (!parser_ok()) return;
  for (auto i = 1; i < cf.constant_pool.size(); i++) {
    TRACE_DO(B, { Trace::col_1(fmt::format("--> entry #{}", i)); });
    parse_constant_pool_entry(cf, i);
  }
}