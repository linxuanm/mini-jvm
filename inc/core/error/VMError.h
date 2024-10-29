#pragma once

#include <string>
#include <utility>

struct VMError {
  // VM phase of the error
  std::string phase;
  // location of the error (file name, byte offset, etc.) (used in testing)
  std::string locator;
  // short description of the error (used in testing)
  std::string kind;
  // readable message of the error
  std::string message;

  [[nodiscard]] std::string format() const;
};
