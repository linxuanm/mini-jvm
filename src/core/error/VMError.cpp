#include "core/error/VMError.h"

#include <sstream>
#include <fmt/core.h>

std::string VMError::format() const {
  std::stringstream ss;
  ss << "VMError during " << phase << std::endl;
  ss << "What went wrong: " << kind << "@" << locator << std::endl;
  ss << message << std::endl;
  return ss.str();
}