#pragma once

#include "core/vmdefs.h"
#include "core/specs/class_flags.h"

#include <string>
#include <vector>

struct CPInfo {
  ConstantPoolTag tag;

  union {
    struct {
      u16 name_index;
    } class_info;

    struct {
      u16 class_index;
      u16 name_and_type_index;
    } ref_info;

    struct {
      u16 string_index;
    } string_info;

    struct {
      u32 bytes;
    } int_float_info;

    struct {
      u64 bytes;
    } long_double_info;

    struct {
      u16 name_index;
      u16 descriptor_index;
    } nameandtype_info;

    struct {
      u16 length;
      u8 *bytes;
    } utf8_info;

    struct {
      u8 reference_kind;
      u16 reference_index;
    } methodhandle_info;

    struct {
      u16 descriptor_index;
    } methodtype_info;

    struct {
      u16 bootstrap_method_attr_index;
      u16 name_and_type_index;
    } dynamic_invokedynamic_info;

    struct {
      u16 name_index;
    } module_package_info;

  } data;
};

struct FieldInfo {};

struct MethodInfo {};

struct AttributeInfo {};

/**
 * Intermediate representation of a class file whose instantiation is part of
 * the binary parsing phase.
 */
struct ClassFile {
  std::vector<CPInfo> constant_pool;
  u16 access_flags;
  u16 this_class;
  u16 super_class;
  std::vector<u16> interfaces;
  std::vector<FieldInfo> fields;
  std::vector<MethodInfo> methods;
  std::vector<AttributeInfo> attributes;

  ~ClassFile() {
    for (auto &[tag, data] : constant_pool) {
      if (tag == CONSTANT_Utf8) free(data.utf8_info.bytes);
    }
  }
};