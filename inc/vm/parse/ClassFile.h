#pragma once

#include "core/vmdefs.h"

#include <vector>

struct CPInfo {};

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
};
