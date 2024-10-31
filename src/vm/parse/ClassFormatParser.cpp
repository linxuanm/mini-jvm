#include "vm/parse/ClassFormatParser.h"

#include "core/specs/class_flags.h"

void ClassFormatParser::traces(pc_t pos,
                               const std::vector<TraceEntry> &entries) const {
  for (auto [tag, size] : entries) {
    parser.trace_bytes(tag, pos, size);
    pos += size;
  }
}

void ClassFormatParser::parse_constant_pool_entry(ClassFile &cf, u16 &i) {
  const pc_t pos    = parser.get_pc();
  const auto cf_tag = static_cast<ConstantPoolTag>(parser.read_u8());
  if (!parser_ok()) return;
  TRACE_DO(B, { parser.trace_bytes(render_CONSTANT(cf_tag), pos, 1); });

  auto &[tag, data] = cf.constant_pool[i];
  tag               = cf_tag;

  switch (tag) {
  case CONSTANT_Utf8:
    data.utf8_info.length = parser.read_wtag_u16("length");
    TRACE_DO(B, { parser.trace_bytes("bytes", data.utf8_info.length); });
    for (auto b = 0; b < data.utf8_info.length; b++) parser.read_u8();
    // TODO: populate string entry
    break;
  case CONSTANT_Integer:
  case CONSTANT_Float:
    data.int_float_info.bytes = parser.read_wtag_u32("bytes");
    break;
  case CONSTANT_Long:
  case CONSTANT_Double:
    data.long_double_info.bytes = parser.read_wtag_u64("bytes");
    i++;
    break;
  case CONSTANT_Class:
    data.class_info.name_index = parser.read_wtag_u16("name index");
    break;
  case CONSTANT_String:
    data.string_info.string_index = parser.read_wtag_u16("string index");
    break;
  case CONSTANT_Fieldref:
  case CONSTANT_Methodref:
  case CONSTANT_InterfaceMethodref:
    data.ref_info.class_index = parser.read_wtag_u16("name index");
    data.ref_info.name_and_type_index = parser.read_wtag_u16(
        "name and type index");
    break;
  case CONSTANT_NameAndType:
    data.nameandtype_info.name_index = parser.read_wtag_u16("name index");
    data.nameandtype_info.descriptor_index = parser.read_wtag_u16(
        "descriptor index");
    break;
  case CONSTANT_MethodHandle:
    data.methodhandle_info.reference_kind = parser.read_wtag_u16(
        "reference kind");
    data.methodhandle_info.reference_index = parser.read_wtag_u16(
        "reference index");
    break;
  case CONSTANT_MethodType:
    data.methodtype_info.descriptor_index = parser.read_wtag_u16(
        "descriptor index");
    break;
  case CONSTANT_Dynamic:
  case CONSTANT_InvokeDynamic:
    data.dynamic_invokedynamic_info.bootstrap_method_attr_index = parser.
        read_wtag_u16("bootstrap method attr index");
    data.dynamic_invokedynamic_info.name_and_type_index = parser.read_wtag_u16(
        "name_and_type index");
    break;
  case CONSTANT_Module:
  case CONSTANT_Package:
    data.module_package_info.name_index = parser.read_wtag_u16("name index");
    break;
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
  const u16 cf_size = parser.read_wtag_u16("constant pool count");
  cf.constant_pool.resize(cf_size);
  if (!parser_ok()) return;
  for (u16 i = 1; i < cf_size; i++) {
    TRACE_DO(B, { Trace::col_1(fmt::format("--> entry #{}", i)); });
    parse_constant_pool_entry(cf, i);
  }
}