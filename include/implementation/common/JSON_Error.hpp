#pragma once

#include <stdexcept>
#include <string>

namespace JSON_Lib {

// JSON error types
struct Error : public std::runtime_error
{
  explicit Error(const std::string &message) : std::runtime_error("JSON Error: " + message) {}
  explicit Error(const std::pair<long, long> &position, const std::string &message = "")
    : std::runtime_error("JSON Error [Line: " + std::to_string(position.first)
                         + " Column: " + std::to_string(position.second) + "]: " + message)
  {}
};
struct SyntaxError : public std::runtime_error
{
  SyntaxError(const std::string &message) : std::runtime_error("JSON Syntax Error: " + message) {}
  explicit SyntaxError(const std::pair<long, long> &position, const std::string &message = "")
    : std::runtime_error("JSON Syntax Error [Line: " + std::to_string(position.first)
                         + " Column: " + std::to_string(position.second) + "]: " + message)
  {}
};
}// namespace JSON_Lib