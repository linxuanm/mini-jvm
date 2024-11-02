#include "vm/parse/ClassFormatParser.h"

#include "core/specs/class_flags.h"

#include <codecvt>

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
  case CONSTANT_Utf8: {
    const auto length = parser.read_wtag_u16("length");
    const auto str = decode_modified_utf8(length);
    data.utf8_info.length = length;
    data.utf8_info.bytes = new char[str.length() + 1];
    std::strncpy(data.utf8_info.bytes, str.c_str(), length + 1);
    break;
  }
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
    data.class_info.name_index = read_cp_index(cf, "name index");
    break;
  case CONSTANT_String:
    data.string_info.string_index = read_cp_index(cf, "string index");
    break;
  case CONSTANT_Fieldref:
  case CONSTANT_Methodref:
  case CONSTANT_InterfaceMethodref:
    data.ref_info.class_index = read_cp_index(cf, "name index");
    data.ref_info.name_and_type_index =
        read_cp_index(cf, "name and type index");
    break;
  case CONSTANT_NameAndType:
    data.nameandtype_info.name_index = read_cp_index(cf, "name index");
    data.nameandtype_info.descriptor_index = read_cp_index(
        cf, "descriptor index");
    break;
  case CONSTANT_MethodHandle:
    data.methodhandle_info.reference_kind = read_cp_index(cf, "reference kind");
    data.methodhandle_info.reference_index = read_cp_index(
        cf, "reference index");
    break;
  case CONSTANT_MethodType:
    data.methodtype_info.descriptor_index = read_cp_index(
        cf, "descriptor index");
    break;
  case CONSTANT_Dynamic:
  case CONSTANT_InvokeDynamic:
    data.dynamic_invokedynamic_info.bootstrap_method_attr_index = read_cp_index(
        cf, "bootstrap method attr index");
    data.dynamic_invokedynamic_info.name_and_type_index = read_cp_index(cf,
      "name_and_type index");
    break;
  case CONSTANT_Module:
  case CONSTANT_Package:
    data.module_package_info.name_index = read_cp_index(cf, "name index");
    break;
  }
  parser_ok();
}

std::string ClassFormatParser::decode_modified_utf8(u16 length) {
  const pc_t start_pc = parser.get_pc();
  std::wstring_convert<std::codecvt_utf8<wchar_t>> cv;
  std::stringstream ss;

  for (auto i = 0; i < length; i++) {
    const u16 x = parser.read_u8();
    if (!parser.ok()) return "";
    if (x & 0x80 == 0) { // 0xxx_xxxx
      ss << static_cast<char>(x);
    } else if (x >> 6 == 0b10) { // 10xx_xxxx
      const u16 y = parser.read_u8();
      if (!parser.ok()) return "";
      const u16 sum = ((x & 0x1F) << 6) + (y & 0x3F);
      ss << cv.to_bytes(sum);
    } else if (x >> 4 == 0xE) { // 1110_xxxx
      const u16 y = parser.read_u8();
      const u16 z = parser.read_u8();
      if (!parser.ok()) return "";
      const u16 sum = ((x & 0xF) << 12) + ((y & 0x3F) << 6) + (z & 0x3F);
      ss << cv.to_bytes(sum);
    } else {
      err_atpc(CF_MalformedUTF8, "invalid UTF-8 encoding (start={})", start_pc);
      return "";
    }
  }

  const auto result = ss.str();
  TRACE_DO(B, { parser.trace_bytes(result, start_pc, length); });
  return result;
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
  if (!parser_ok()) return;
  cf.constant_pool.resize(cf_size);
  for (u16 i = 1; i < cf_size; i++) {
    TRACE_DO(B, { Trace::col_1(fmt::format("--> entry #{}", i)); });
    parse_constant_pool_entry(cf, i);
    if (!ok()) return;
  }

  TRACE_DO(B, { Trace::mid_bar_text("Class Info"); });
  // TODO: index validation
  cf.access_flags = parser.read_wtag_u16("access flags");
  cf.this_class   = read_cp_index(cf, "this class");
  cf.super_class  = read_cp_index(cf, "super class", true);
  if (!ok()) return;

  TRACE_DO(B, { Trace::mid_bar_text("Interfaces"); });
  const u16 itf_size = parser.read_wtag_u16("interface count");
  cf.interfaces.resize(itf_size);
  if (!parser_ok()) return;
  for (u16 i = 0; i < itf_size; i++) {
    // TODO: format interface
    cf.interfaces[i] = parser.read_wtag_u16("interface index");
    if (!parser_ok()) return;
  }
}