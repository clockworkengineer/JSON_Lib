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
  explicit JNodeNumeric(const std::string &number) : value(number) {}
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
  // Is character a valid numeric character ?
  // Includes possible sign, decimal point or exponent
  [[nodiscard]] static bool isValidNumericChar(char ch) {
    return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' ||
            ch == 'E' || ch == 'e');
  }
  // Is number a int/long/long long/float/double/long double ?
  [[nodiscard]] bool isInt() const {
    try {
      std::size_t end;
      [[maybe_unused]] auto integer = std::stoi(value.c_str(), &end, 10);
      if (end != value.size()) {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
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
  [[nodiscard]] bool isFloat() const {
    try {
      std::size_t end;
      [[maybe_unused]] auto fp = std::stof(value.c_str(), &end);
      if (end != value.size()) {
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
  // Convert to int/long/long long.Note: Can still return a long value
  // for floating point.
  [[nodiscard]] int getInt() const {
    return (std::stoi(value.c_str(), nullptr, 10));
  }
  [[nodiscard]] long getLong() const {
    return (std::strtol(value.c_str(), nullptr, 10));
  }
  [[nodiscard]] long long getLongLong() const {
    return (std::strtoll(value.c_str(), nullptr, 10));
  }
  // Convert to float/double/long double.
  [[nodiscard]] float getFloat() const {
    return (std::stof(value.c_str(), nullptr));
  }
  [[nodiscard]] double getDouble() const {
    return (std::strtod(value.c_str(), nullptr));
  }
  [[nodiscard]] long double getLongDouble() const {
    return (std::strtold(value.c_str(), nullptr));
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    return (isInt() || isLong() || isLongLong() || isFloat() || isDouble() || isLongDouble());
  }
  // Get string representation of numeric
  [[nodiscard]] std::string getString() const { return (value); }

private:
  std::string value{};
};
} // namespace JSONLib