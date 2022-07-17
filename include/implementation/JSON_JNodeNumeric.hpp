#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =============
// JNode Numeric
// =============
struct JNodeNumeric {
  // Number type
  enum class NumberType {
    tInt = 0,
    tLong,
    tLongLong,
    tFloat,
    tDouble,
    tLongDouble
  };
  // Constructors/Destructors
  JNodeNumeric() = default;
  explicit JNodeNumeric(const std::string &number) : m_value(number) {}
  explicit JNodeNumeric(int integer) {
    m_value = std::to_string(integer);
    m_type = NumberType::tInt;
  }
  explicit JNodeNumeric(long integer) {
    m_value = std::to_string(integer);
    m_type = NumberType::tLong;
  }
  explicit JNodeNumeric(long long integer) {
    m_value = std::to_string(integer);
    m_type = NumberType::tLongLong;
  }
  explicit JNodeNumeric(float floatingPoint) {
    m_value = std::to_string(floatingPoint);
    m_value.erase(m_value.find_last_not_of('0') + 1, std::string::npos);
    if (m_value.back() == '.') {
      m_value += '0';
    }
    m_type = NumberType::tFloat;
  }
  explicit JNodeNumeric(double floatingPoint) {
    m_value = std::to_string(floatingPoint);
    m_value.erase(m_value.find_last_not_of('0') + 1, std::string::npos);
    if (m_value.back() == '.') {
      m_value += '0';
    }
    m_type = NumberType::tDouble;
  }
  explicit JNodeNumeric(long double floatingPoint) {
    m_value = std::to_string(floatingPoint);
    m_value.erase(m_value.find_last_not_of('0') + 1, std::string::npos);
    if (m_value.back() == '.') {
      m_value += '0';
    }
    m_type = NumberType::tLongDouble;
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
      [[maybe_unused]] auto integer = std::stoi(m_value.c_str(), &end, 10);
      if (end != m_value.size()) {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tInt;
    return (true);
  }
  [[nodiscard]] bool isLong() const {
    try {
      char *end;
      std::strtol(m_value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tLong;
    return (true);
  }
  [[nodiscard]] bool isLongLong() const {
    try {
      char *end;
      std::strtoll(m_value.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tLongLong;
    return (true);
  }
  [[nodiscard]] bool isFloat() const {
    try {
      std::size_t end;
      [[maybe_unused]] auto fp = std::stof(m_value.c_str(), &end);
      if (end != m_value.size()) {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tFloat;
    return (true);
  }
  [[nodiscard]] bool isDouble() const {
    try {
      char *end;
      std::strtod(m_value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tDouble;
    return (true);
  }
  [[nodiscard]] bool isLongDouble() const {
    try {
      char *end;
      std::strtold(m_value.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tLongDouble;
    return (true);
  }
  // Convert to int/long/long long.Note: Can still return a long value
  // for floating point.
  [[nodiscard]] int getInt() const {
    return (std::stoi(m_value.c_str(), nullptr, 10));
  }
  [[nodiscard]] long getLong() const {
    return (std::strtol(m_value.c_str(), nullptr, 10));
  }
  [[nodiscard]] long long getLongLong() const {
    return (std::strtoll(m_value.c_str(), nullptr, 10));
  }
  // Convert to float/double/long double.
  [[nodiscard]] float getFloat() const {
    return (std::stof(m_value.c_str(), nullptr));
  }
  [[nodiscard]] double getDouble() const {
    return (std::strtod(m_value.c_str(), nullptr));
  }
  [[nodiscard]] long double getLongDouble() const {
    return (std::strtold(m_value.c_str(), nullptr));
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    return (isInt() || isLong() || isLongLong() || isFloat() || isDouble() ||
            isLongDouble());
  }
  // Get string representation of numeric
  [[nodiscard]] std::string getString() const { return (m_value); }

private:
  mutable NumberType m_type;
  std::string m_value{};
};
} // namespace JSONLib