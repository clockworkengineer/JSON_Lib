#pragma once

namespace JSON_Lib {

// JSON error types
struct Error final : std::runtime_error
{
  explicit Error(const std::string_view &message) : std::runtime_error(std::string("JSON Error: ").append(message)) {}
  explicit Error(const std::pair<long, long> &position, const std::string_view &message = "")
    : std::runtime_error("JSON Error [Line: " + std::to_string(position.first)
                         + " Column: " + std::to_string(position.second) + std::string("]: ").append(message))
  {}
};
struct SyntaxError final : std::runtime_error
{
  explicit SyntaxError(const std::string_view &message) : std::runtime_error(std::string("JSON Syntax Error: ").append(message)) {}
  explicit SyntaxError(const std::pair<long, long> &position, const std::string_view &message = "")
    : std::runtime_error("JSON Syntax Error [Line: " + std::to_string(position.first)
                         + " Column: " + std::to_string(position.second) + std::string("]: ").append(message))
  {}
};
}// namespace JSON_Lib