#include "core/specs/class_flags.h"

#define TABLE_SIZE(table) (sizeof(table) / sizeof(TableEntry))
#define IN_RANGE(table, tag) ((tag) >= 0 && (tag) < TABLE_SIZE(table))

TableEntry class_access_flag_table[] = {
  [CA_ACC_PUBLIC] = {"ACC_PUBLIC"},
  [CA_ACC_FINAL] = {"ACC_FINAL"},
  [CA_ACC_SUPER] = {"ACC_SUPER"},
  [CA_ACC_INTERFACE] = {"ACC_INTERFACE"},
  [CA_ACC_ABSTRACT] = {"ACC_ABSTRACT"},
  [CA_ACC_SYNTHETIC] = {"ACC_SYNTHETIC"},
  [CA_ACC_ANNOTATION] = {"ACC_ANNOTATION"},
  [CA_ACC_ENUM] = {"ACC_ENUM"},
};

TableEntry cp_tag_table[] = {
  [CONSTANT_Utf8] = {"CONSTANT_Utf8"},
  [CONSTANT_Integer] = {"CONSTANT_Integer"},
  [CONSTANT_Float] = {"CONSTANT_Float"},
  [CONSTANT_Long] = {"CONSTANT_Long"},
  [CONSTANT_Double] = {"CONSTANT_Double"},
  [CONSTANT_Class] = {"CONSTANT_Class"},
  [CONSTANT_String] = {"CONSTANT_String"},
  [CONSTANT_Fieldref] = {"CONSTANT_Fieldref"},
  [CONSTANT_Methodref] = {"CONSTANT_Methodref"},
  [CONSTANT_InterfaceMethodref] = {"CONSTANT_InterfaceMethodref"},
  [CONSTANT_NameAndType] = {"CONSTANT_NameAndType"},
  [CONSTANT_MethodHandle] = {"CONSTANT_MethodHandle"},
  [CONSTANT_MethodType] = {"CONSTANT_MethodType"},
  [CONSTANT_InvokeDynamic] = {"CONSTANT_InvokeDynamic"}
};

const char *render_ACC(ClassAccessFlag ca) {
  if (IN_RANGE(class_access_flag_table, ca) && class_access_flag_table[ca].name != 0)
    return class_access_flag_table[ca].name;
  return "UNKNOWN";
}

const char *render_CONSTANT(ConstantPoolTag cpt) {
  if (IN_RANGE(cp_tag_table, cpt) && cp_tag_table[cpt].name != 0)
    return cp_tag_table[cpt].name;
  return "UNKNOWN";
}
