#pragma once

typedef struct {
  const char *name;
} TableEntry;

typedef enum {
  CA_ACC_PUBLIC = 0x0001,
  CA_ACC_FINAL = 0x0010,
  CA_ACC_SUPER = 0x0020,
  CA_ACC_INTERFACE = 0x0200,
  CA_ACC_ABSTRACT = 0x0400,
  CA_ACC_SYNTHETIC = 0x1000,
  CA_ACC_ANNOTATION = 0x2000,
  CA_ACC_ENUM = 0x4000
} ClassAccessFlag;

typedef enum {
  CONSTANT_Utf8 = 1,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,
  CONSTANT_MethodHandle = 15,
  CONSTANT_MethodType = 16,
  CONSTANT_InvokeDynamic = 18
} ConstantPoolTag;

typedef enum {
  REF_getField = 1,
  REF_getStatic = 2,
  REF_putField = 3,
  REF_putStatic = 4,
  REF_invokeVirtual = 5,
  REF_invokeStatic = 6,
  REF_invokeSpecial = 7,
  REF_newInvokeSpecial = 8,
  REF_invokeInterface = 9
} ReferenceKind;

extern TableEntry class_access_flag_table[];

// constexpr ClassAccessFlag class_access_flags[] = {
//   CA_ACC_PUBLIC,
//   CA_ACC_FINAL,
//   CA_ACC_SUPER,
//   CA_ACC_INTERFACE,
//   CA_ACC_ABSTRACT,
//   CA_ACC_SYNTHETIC,
//   CA_ACC_ANNOTATION,
//   CA_ACC_ENUM,
//   {}
// };

extern TableEntry cp_tag_table[];

// constexpr ConstantPoolTag cp_tags[] = {
//   CONSTANT_Utf8,
//   CONSTANT_Integer,
//   CONSTANT_Float,
//   CONSTANT_Long,
//   CONSTANT_Double,
//   CONSTANT_Class,
//   CONSTANT_String,
//   CONSTANT_Fieldref,
//   CONSTANT_Methodref,
//   CONSTANT_InterfaceMethodref,
//   CONSTANT_NameAndType,
//   CONSTANT_MethodHandle,
//   CONSTANT_MethodType,
//   CONSTANT_InvokeDynamic,
//   {}
// };

extern TableEntry ref_kind_table[];

#ifdef __cplusplus
extern "C" {
#endif

const char *render_ACC(ClassAccessFlag ca);
const char *render_CONSTANT(ConstantPoolTag cpt);

#ifdef __cplusplus
}
#endif
