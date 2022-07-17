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
  explicit JNodeNumeric(int integer) { value = std::to_string(integer); }
  explicit JNodeNumeric(long integer) { value = std::to_string(integer); }
  explicit JNodeNumeric(long long integer) { value = std::to_string(integer); }
  explicit JNodeNumeric(float floatingPoint) {
    value = std::to_string(floatingPoint);
    value.erase(value.find_last_not_of('0') + 1, std::string::npos);
    if (value.back() == '.') {
      value += '0';
    }
  }
  explicit JNodeNumeric(double floatingPoint) {
    value = std::to_string(floatingPoint);
    value.erase(value.find_last_not_of('0') + 1, std::string::npos);
    if (value.back() == '.') {
      value += '0';
    }
  }
  explicit JNodeNumeric(long double floatingPoint) {
    value = std::to_string(floatingPoint);
    value.erase(value.find_last_not_of('0') + 1, std::string::npos);
    if (value.back() == '.') {
      value += '0';
    }
  }
  JNodeNumeric(const JNodeNumeric &other) = default;
  JNodeNumeric &operator=(const JNodeNumeric &other) = default;
  JNodeNumeric(JNodeNumeric &&other) = default;
  JNodeNumeric &operator=(JNodeNumeric &&other) = default;
  ~JNodeNumeric() = default;
  [[nodiscard]] bool isLong() const {
    try {
      char *end;
      std::strtol(value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool isLongLong() const {
    try {
      char *end;
      std::strtoll(value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool isDouble() const {
    try {
      char *end;
      std::strtod(value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
    [[nodiscard]] bool isLongDouble() const {
    try {
      char *end;
      std::strtold(value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Is character a valid numeric character ?
  [[nodiscard]] bool isValidNumericChar(char ch) const {
    // Includes possible sign, decimal point or exponent
    return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' ||
            ch == 'E' || ch == 'e');
  }
  // Convert to long/long long returning true on success
  // Note: Can still return a long value for floating point
  // but false as the number is not in integer format
  [[nodiscard]] bool integer(long &integerValue) const {
    try {
      char *end;
      integerValue = std::strtol(value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool integer(long long &integerValue) const {
    try {
      char *end;
      integerValue = std::strtoll(value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Convert to double/long double returning true on success
  [[nodiscard]] bool floatingpoint(double &doubleValue) const {
    try {
      char *end;
      doubleValue = std::strtod(value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool floatingpoint(long double &doubleValue) const {
    try {
      char *end;
      doubleValue = std::strtold(value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    return(isLong()||isLongLong()||isDouble()||isLongDouble());
  }
  std::string value{};
};
} // namespace JSONLib