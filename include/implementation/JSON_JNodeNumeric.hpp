#pragma once
// =======
// C++ STL
// =======
#include <array>
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =============
// JNode Numeric
// =============
struct JNodeNumeric {
  // Constructors/Destructors
  JNodeNumeric() = default;
  explicit JNodeNumeric(int integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long long integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(float floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(double floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long double floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  JNodeNumeric(const JNodeNumeric &other) = default;
  JNodeNumeric &operator=(const JNodeNumeric &other) = default;
  JNodeNumeric(JNodeNumeric &&other) = default;
  JNodeNumeric &operator=(JNodeNumeric &&other) = default;
  ~JNodeNumeric() = default;
  // Convert to long/long long returning true on success
  // Note: Can still return a long value for floating point
  // but false as the number is not in integer format
  [[nodiscard]] bool integer(long &integerValue) const {
    try {
      integerValue = std::stol(&value[0], nullptr);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool integer(long long &integerValue) const {
    try {
      integerValue = std::stoll(&value[0], nullptr);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Convert to double/long double returning true on success
  [[nodiscard]] bool floatingpoint(double &doubleValue) const {
    try {
      doubleValue = std::strtod(&value[0], nullptr);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool floatingpoint(long double &doubleValue) const {
    try {
      doubleValue = std::strtold(&value[0], nullptr);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    if ([[maybe_unused]] long longValue{}; integer(longValue)) {
      return (true);
    }
    if ([[maybe_unused]] long long longValue{}; integer(longValue)) {
      return (true);
    }
    if ([[maybe_unused]] double doubleValue{}; floatingpoint(doubleValue)) {
      return (true);
    }
    if ([[maybe_unused]] long double doubleValue{};
        floatingpoint(doubleValue)) {
      return (true);
    }
    return (false);
  }
  // Numeric values max width in characters
  static constexpr int kLongLongWidth =
      std::numeric_limits<long long>::digits10 + 2;
  static constexpr int kLongDoubleWidth =
      std::numeric_limits<long double>::digits10 + 2;
  // Numbers stored in fixed length array
  std::array<char, kLongLongWidth> value{};
};
} // namespace JSONLib