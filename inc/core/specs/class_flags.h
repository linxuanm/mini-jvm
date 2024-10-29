#pragma once

enum ClassAccess {
  ACC_PUBLIC = 0x0001,
  ACC_FINAL = 0x0010,
  ACC_SUPER = 0x0020,
  ACC_INTERFACE = 0x0200,
  ACC_ABSTRACT = 0x0400,
  ACC_SYMTHETIC = 0x1000,
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