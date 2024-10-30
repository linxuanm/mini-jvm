#pragma once

enum ClassAccess {
  ACC_PUBLIC = 0x0001,
  ACC_FINAL = 0x0010,
  ACC_SUPER = 0x0020,
  ACC_INTERFACE = 0x0200,
  ACC_ABSTRACT = 0x0400,
  ACC_SYNTHETIC = 0x1000,
  ACC_ANNOTATION = 0x2000,
  ACC_ENUM = 0x4000,
  ACC_MODULE = 0x8000
};

enum ConstantPoolTag {
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
  CONSTANT_Dynamic = 17,
  CONSTANT_InvokeDynamic = 18,
  CONSTANT_Module = 19,
  CONSTANT_Package = 20
};

inline const char *render_ACC(ClassAccess ca) {
  switch (ca) {
  case ACC_PUBLIC: return "ACC_PUBLIC";
  case ACC_FINAL: return "ACC_FINAL";
  case ACC_SUPER: return "ACC_SUPER";
  case ACC_INTERFACE: return "ACC_INTERFACE";
  case ACC_ABSTRACT: return "ACC_ABSTRACT";
  case ACC_SYNTHETIC: return "ACC_SYNTHETIC";
  case ACC_ANNOTATION: return "ACC_ANNOTATION";
  case ACC_ENUM: return "ACC_ENUM";
  case ACC_MODULE: return "ACC_MODULE";
  default: return "UNKNOWN";
  }
}

inline const char *render_CONSTANT(ConstantPoolTag cpt) {
  switch (cpt) {
  case CONSTANT_Utf8: return "CONSTANT_Utf8";
  case CONSTANT_Integer: return "CONSTANT_Integer";
  case CONSTANT_Float: return "CONSTANT_Float";
  case CONSTANT_Long: return "CONSTANT_Long";
  case CONSTANT_Double: return "CONSTANT_Double";
  case CONSTANT_Class: return "CONSTANT_Class";
  case CONSTANT_String: return "CONSTANT_String";
  case CONSTANT_Fieldref: return "CONSTANT_Fieldref";
  case CONSTANT_Methodref: return "CONSTANT_Methodref";
  case CONSTANT_InterfaceMethodref: return "CONSTANT_InterfaceMethodref";
  case CONSTANT_NameAndType: return "CONSTANT_NameAndType";
  case CONSTANT_MethodHandle: return "CONSTANT_MethodHandle";
  case CONSTANT_MethodType: return "CONSTANT_MethodType";
  case CONSTANT_Dynamic: return "CONSTANT_Dynamic";
  case CONSTANT_InvokeDynamic: return "CONSTANT_InvokeDynamic";
  case CONSTANT_Module: return "CONSTANT_Module";
  case CONSTANT_Package: return "CONSTANT_Package";
  default: return "UNKNOWN";
  }
}
