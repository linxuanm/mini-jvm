#pragma once

#include <string>
#include <utility>

class VMError {

public:
  explicit VMError(std::string p): phase{std::move(p)} {
  }

  [[nodiscard]] virtual std::string locator() const = 0;
  [[nodiscard]] virtual std::string desc() const = 0;

protected:
  ~VMError() = default;

private:
  std::string phase;

};