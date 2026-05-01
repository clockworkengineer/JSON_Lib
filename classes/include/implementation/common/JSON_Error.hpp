#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace JSON_Lib {

enum class Status : uint8_t
{
  Ok = 0,
  SyntaxError,
  OutOfMemory,
  InvalidKey,
  InvalidIndex,
  UnsupportedEncoding,
  InvalidInput,
  NoData,
  UnknownError
};

template<typename T>
struct Result
{
  Status status{Status::Ok};
  std::unique_ptr<T> value;
  std::string message;
  std::pair<long, long> position{0, 0};

  [[nodiscard]] bool ok() const noexcept { return status == Status::Ok; }
  [[nodiscard]] T &unwrap() { return *value; }
  [[nodiscard]] const T &unwrap() const { return *value; }
};

template<>
struct Result<void>
{
  Status status{Status::Ok};
  std::string message;
  std::pair<long, long> position{0, 0};

  [[nodiscard]] bool ok() const noexcept { return status == Status::Ok; }
};

// JSON error types
inline std::string formatPosition(const std::pair<long, long> &position, const std::string_view prefix)
{
  return std::string(prefix) + " [Line: " + std::to_string(position.first)
       + " Column: " + std::to_string(position.second) + "]: ";
}
struct Error final : std::runtime_error
{
  explicit Error(const std::string_view &message) : std::runtime_error(std::string("JSON Error: ").append(message)) {}
  explicit Error(const std::pair<long, long> &position, const std::string_view &message = "")
    : std::runtime_error(formatPosition(position, "JSON Error").append(message))
  {}
};
struct SyntaxError final : std::runtime_error
{
  explicit SyntaxError(const std::string_view &message) : std::runtime_error(std::string("JSON Syntax Error: ").append(message)) {}
  explicit SyntaxError(const std::pair<long, long> &position, const std::string_view &message = "")
    : std::runtime_error(formatPosition(position, "JSON Syntax Error").append(message))
  {}
};
}// namespace JSON_Lib