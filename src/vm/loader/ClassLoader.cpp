#include "vm/loader/ClassLoader.h"

#include "core/vmdefs.h"

#include <fstream>
#include <iterator>
#include <fmt/core.h>

CLResult ClassLoader::load_class_file(const std::string &path) {
  std::ifstream file(path, std::ios::binary);
  if (file.fail())
    return {CL_FileNotFound};

  const ByteArray buf((std::istream_iterator<u8>(file)),
                      std::istream_iterator<u8>());

  ClassFile cf;
  cfparser.parse(cf, &buf);
  if (!cfparser.ok()) {
    return {CL_FileFormatError, cfparser.error()};
  }

  return {CL_Ok};
}