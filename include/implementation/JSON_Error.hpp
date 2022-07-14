
#pragma once
// =======
// C++ STL
// =======
#include <stdexcept>
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==========
// JSON Error
// ==========
struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("JSON Error: " + message) {}
};
} // namespace JSONLib