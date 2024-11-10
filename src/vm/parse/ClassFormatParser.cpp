#include "vm/parse/ClassFormatParser.h"

#include "core/specs/class_flags.h"
#include "core/trace.h"
#include "fmt/xchar.h"

#include <codecvt>
#include <locale>

void ClassFormatParser::traces(pc_t pos, const std::vector<TraceEntry> &entries) const {
  for (auto [tag, size] : entries) {
    parser.trace_bytes(tag, pos, size);
    pos += size;
  }
}

void ClassFormatParser::parse_constant_pool_entry(u16 &i) {
  const pc_t pos = parser.get_pc();
  const auto cf_tag = static_cast<ConstantPoolTag>(parser.read_u8());
  if (!parser_ok()) return;
  TRACE_DO(B, { parser.trace_bytes(render_CONSTANT(cf_tag), pos, 1); });

  auto &[tag, data] = cf->constant_pool[i];
  tag = cf_tag;

  switch (tag) {
  case CONSTANT_Utf8: {
    const auto length = parser.read_wtag_u16("length");
    const auto str = decode_modified_utf8(length);
    data.utf8_info.length = length;
    data.utf8_info.bytes = new char[str.length() + 1];
    std::strncpy(data.utf8_info.bytes, str.c_str(), length + 1);
    const auto pretty = fmt::format("\"{}\"", str);
    TRACE_DO(B, { parser.trace_bytes(pretty, pos + 2, length); });
    break;
  }
  case CONSTANT_Integer: {
    auto val = parser.read_i32();
    data.int_info.val = val;
    TRACE_DO(B, { parser.trace_bytes(fmt::format("{}", val), pos, 4); });
    break;
  }
  case CONSTANT_Float: {
    auto val = std::bit_cast<float>(parser.read_u32());
    data.float_info.val = val;
    TRACE_DO(B, { parser.trace_bytes(fmt::format("{}f", val), pos, 4); });
    break;
  }
  case CONSTANT_Long: {
    auto val = parser.read_i64();
    data.long_info.val = val;
    TRACE_DO(B, { parser.trace_bytes(fmt::format("{}l", val), pos, 8); });
    i++;
    break;
  }
  case CONSTANT_Double: {
    auto val = std::bit_cast<double>(parser.read_u64());
    data.double_info.val = val;
    TRACE_DO(B, { parser.trace_bytes(fmt::format("{}d", val), pos, 8); });
    i++;
    break;
  }
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
    data.ref_info.name_and_type_index = parser.read_wtag_u16("name and type index");
    break;
  case CONSTANT_NameAndType:
    data.nameandtype_info.name_index = parser.read_wtag_u16("name index");
    data.nameandtype_info.descriptor_index = parser.read_wtag_u16("descriptor index");
    break;
  case CONSTANT_MethodHandle:
    data.methodhandle_info.reference_kind = parser.read_wtag_u16("reference kind");
    data.methodhandle_info.reference_index = parser.read_wtag_u16("reference index");
    break;
  case CONSTANT_MethodType:
    data.methodtype_info.descriptor_index = parser.read_wtag_u16("descriptor index");
    break;
  case CONSTANT_InvokeDynamic:
    data.invokedynamic_info.bootstrap_method_attr_index = parser.read_wtag_u16(
        "bootstrap method attr index");
    data.invokedynamic_info.name_and_type_index = parser.read_wtag_u16("name_and_type index");
    break;
  }
  parser_ok();
}

bool ClassFormatParser::verify_cp_index(u16 index, ConstantPoolTag tag) {
  if (index == 0 || index >= cf->constant_pool.size()) {
    err_atpc(CF_InvalidIndex, "constant pool index must be in range [1, {}], got {}",
             cf->constant_pool.size() - 1, index);
    return false;
  }
  const auto &info = cf->constant_pool[index];
  if (info.tag != tag) {
    err_atpc(CF_MismatchedCPType, "expected {} at constant_pool[{}], got {}", render_CONSTANT(tag),
             index, render_CONSTANT(info.tag));
    return false;
  }
  return true;
}

void ClassFormatParser::verify_constant_pool_entry(u16 &i) {
  const auto &[tag, data] = cf->constant_pool[i];
  switch (tag) {
  case CONSTANT_Utf8:
  case CONSTANT_Integer:
  case CONSTANT_Float:
  case CONSTANT_String:
    break;
  case CONSTANT_Long:
  case CONSTANT_Double:
    i++;
    break;
  case CONSTANT_Class:
    verify_cp_index(data.class_info.name_index, CONSTANT_Utf8);
    break;
  case CONSTANT_Fieldref:
  case CONSTANT_Methodref:
  case CONSTANT_InterfaceMethodref:
    verify_cp_index(data.ref_info.class_index, CONSTANT_Class);
    verify_cp_index(data.ref_info.name_and_type_index, CONSTANT_NameAndType);
    break;
  case CONSTANT_NameAndType:
    verify_cp_index(data.nameandtype_info.name_index, CONSTANT_Utf8);
    verify_cp_index(data.nameandtype_info.descriptor_index, CONSTANT_Utf8);
    break;
  case CONSTANT_MethodHandle:
    // TODO
    break;
  case CONSTANT_MethodType:
    verify_cp_index(data.methodtype_info.descriptor_index, CONSTANT_Utf8);
    break;
  case CONSTANT_InvokeDynamic:
    cp_bootstrap_forward_refs.push_back(data.invokedynamic_info.bootstrap_method_attr_index);
    verify_cp_index(data.invokedynamic_info.name_and_type_index, CONSTANT_NameAndType);
    break;
  }
}

void ClassFormatParser::parse_field_entry(u16 &i) {}

std::string ClassFormatParser::decode_modified_utf8(u16 length) {
  const pc_t start_pc = parser.get_pc();
  std::wstring_convert<std::codecvt_utf8<wchar_t>> cv;
  std::stringstream ss;

  for (auto i = 0; i < length; i++) {
    const u16 x = parser.read_u8();
    if (!parser.ok()) return "";
    if ((x & 0x80) == 0) {
      // 0xxx_xxxx
      ss << static_cast<char>(x);
    } else if (x >> 6 == 0b10) {
      // 10xx_xxxx
      const u16 y = parser.read_u8();
      if (!parser.ok()) return "";
      const u16 sum = ((x & 0x1F) << 6) + (y & 0x3F);
      ss << cv.to_bytes(sum);
    } else if (x >> 4 == 0xE) {
      // 1110_xxxx
      const u16 y = parser.read_u8();
      const u16 z = parser.read_u8();
      if (!parser.ok()) return "";
      const u16 sum = ((x & 0xF) << 12) + ((y & 0x3F) << 6) + (z & 0x3F);
      ss << cv.to_bytes(sum);
    } else {
      err_atpc(CF_MalformedUTF8, "invalid UTF-8 encoding {:02X} (start={})", x, start_pc);
      return "";
    }
  }

  return ss.str();
}

u16 ClassFormatParser::read_cp_index(const char *msg, ConstantPoolTag tag, bool print) {
  const pc_t pos = parser.get_pc();
  const u16 index = parser.read_u16();
  if (!parser.ok()) return 0;

  if (index == 0 || index >= cf->constant_pool.size()) {
    err_atpc(CF_InvalidIndex, "constant pool index must be in range [1, {}], got {}",
             cf->constant_pool.size() - 1, index);
    return 0;
  }

  if (print) {
    TRACE_DO(B, {
             const auto s = format_cp_index(index);
             parser.trace_bytes(fmt::format("{}: {}", msg, s), pos, 2);
             });
  } else { TRACE_DO(B, { parser.trace_bytes(msg, pos, 2); }); }
  return index;
}

std::string ClassFormatParser::format_cp_index(u16 i) const {
  const auto &[tag, data] = cf->constant_pool[i];
  switch (tag) {
  case CONSTANT_Utf8:
    return fmt::format("{}", data.utf8_info.bytes);
  case CONSTANT_Integer:
    return fmt::format("{}", static_cast<int>(data.int_info.val));
  case CONSTANT_Float:
    return fmt::format("{}f", data.float_info.val);
  case CONSTANT_Long:
    return fmt::format("{}l", data.long_info.val);
  case CONSTANT_Double:
    return fmt::format("{}d", data.double_info.val);
  case CONSTANT_Class:
    return format_cp_index(data.class_info.name_index);
  case CONSTANT_String:
    return fmt::format("\"{}\"", format_cp_index(data.class_info.name_index));
  case CONSTANT_Fieldref:
  case CONSTANT_Methodref:
  case CONSTANT_InterfaceMethodref: {
    const auto &name = format_cp_index(data.ref_info.class_index);
    const auto &desc = format_cp_index(data.ref_info.name_and_type_index);
    return fmt::format("{}.{}", name, desc);
  }
  case CONSTANT_NameAndType: {
    const auto &name = format_cp_index(data.nameandtype_info.name_index);
    const auto &type = format_cp_index(data.nameandtype_info.descriptor_index);
    return fmt::format("{}:{}", name, type);
  }
  case CONSTANT_MethodHandle:
    break;
  case CONSTANT_MethodType:
    break;
  case CONSTANT_InvokeDynamic:
    break;
  }
  return "";
}

void ClassFormatParser::parse(ClassFile *in_cf, ByteArrayRef ref) {
  cf = in_cf;
  parser.reset(ref, 0);

  const u32 magic = parser.read_wtag_u32("magic");
  if (!parser_ok()) return;
  if (magic != 0xCAFEBABE) return err_atpc(CF_NotAClassFile, "invalid header 0x{:X}", magic);

  parser.read_wtag_u16("minor version");
  const u16 major = parser.read_wtag_u16("major version");
  if (!parser_ok()) return;
  if (major != 52) return err_atpc(CF_VersionMismatch, "require version 52, got {}", major);

  TRACE_DO(B, { Trace::mid_bar_text("Constant Pool"); });
  const u16 cf_size = parser.read_wtag_u16("constant pool count");
  if (!parser_ok()) return;
  cf->constant_pool.resize(cf_size);
  for (u16 i = 1; i < cf_size; i++) {
    TRACE_DO(B, { Trace::col_1(fmt::format("--> entry #{}", i)); });
    parse_constant_pool_entry(i);
    if (!ok()) return;
  }
  for (u16 i = 1; i < cf_size; i++) {
    verify_constant_pool_entry(i);
    if (!ok()) return;
  }

  TRACE_DO(B, { Trace::mid_bar_text("Class Info"); });
  cf->access_flags = parser.read_wtag_u16("access flags");
  cf->this_class = read_cp_index("this class", CONSTANT_Class, true);
  const pc_t pre_super = parser.get_pc();
  const bool is_object = parser.read_u16() == 0;
  parser.reset(pre_super);
  if (is_object) cf->super_class = parser.read_wtag_u16("super class: NONE");
  else cf->super_class = read_cp_index("super class", CONSTANT_Class, true);
  if (!parser_ok() || !ok()) return;

  TRACE_DO(B, { Trace::mid_bar_text("Interfaces"); });
  const u16 itf_size = parser.read_wtag_u16("interface count");
  if (!parser_ok()) return;
  cf->interfaces.resize(itf_size);
  for (u16 i = 0; i < itf_size; i++) {
    cf->interfaces[i] = parser.read_wtag_u16("interface index");
    if (!parser_ok()) return;
  }

  TRACE_DO(B, { Trace::mid_bar_text("Fields"); });
  const u16 fields_size = parser.read_wtag_u16("fields count");
  if (!parser_ok()) return;
  cf->fields.resize(fields_size);
  for (u16 i = 0; i < fields_size; i++) {
    TRACE_DO(B, { Trace::col_1(fmt::format("--> entry #{}", i)); });
    parse_field_entry(i);
    if (!parser_ok()) return;
  }
}