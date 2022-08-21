
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
struct Error : public std::runtime_error
{
  explicit Error(const std::string &message) : std::runtime_error("JSON Error: " + message) {}
  explicit Error(const std::pair<long, long> &position, const std::string &message = "")
    : std::runtime_error("JSON Error [Line: " + std::to_string(position.first)
                         + " Column: " + std::to_string(position.second) + "]: " + message)
  {}
};
}// namespace JSONLib