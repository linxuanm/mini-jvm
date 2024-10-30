#include "vm/loader/ClassLoader.h"

#include "core/vmdefs.h"

#include <fstream>
#include <iterator>
#include <fmt/core.h>

CLResult ClassLoader::load_class_file(const std::string &path) {
  std::ifstream file(path, std::ios::binary);
  if (file.fail())
    return {CL_FileNotFound};

  file.seekg(0, std::ios::end);
  std::streampos size = file.tellg();
  file.seekg(0, std::ios::beg);

  ByteArray buf(size);
  file.read(reinterpret_cast<char*>(buf.data()), size);

  ClassFile cf;
  TRACE_DO(B, { Trace::title(fmt::format("[Binary] {}", path)); });
  cfparser.parse(cf, &buf);
  if (!cfparser.ok()) { return {CL_FileFormatError, cfparser.error()}; }

  return {CL_Ok};
}