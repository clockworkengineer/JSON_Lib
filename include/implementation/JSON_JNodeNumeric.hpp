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
  // Is number a long/long long/double/long double ?
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
  // Convert to long/long long.Note: Can still return a long value
  // for floating point.
  [[nodiscard]] long getLong() const {
    return (std::strtol(value.c_str(), nullptr, 10));
  }
  [[nodiscard]] long long getLongLong() const {
    return (std::strtoll(value.c_str(), nullptr, 10));
  }
  // Convert to double/long double.
  [[nodiscard]] double getDouble() const {
    return (std::strtod(value.c_str(), nullptr));
  }
  [[nodiscard]] long double getLongDouble() const {
      return(std::strtold(value.c_str(), nullptr));
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    return (isLong() || isLongLong() || isDouble() || isLongDouble());
  }
  std::string value{};
};
} // namespace JSONLib