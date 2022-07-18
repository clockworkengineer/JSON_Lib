#pragma once
// =======
// C++ STL
// =======
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ================
// Numeric concepts
// ================
template <typename T>
concept Integer = std::is_integral<T>::value;
template <typename T>
concept Float = std::is_floating_point<T>::value;
// =============
// JNode Numeric
// =============
struct JNodeNumeric {
  // Shortened type names
  using llong = long long;
  using ldouble = long double;
  // Number to string
  template <Integer T> std::string numericToString(const T &t) const {
    std::ostringstream os;
    os << t;
    return os.str();
  }
  template <Float T> std::string numericToString(const T &t) const {
    std::ostringstream os;
    os << t;
    std::string floatString = os.str();
    if (floatString.find(".") == std::string::npos) {
      floatString.push_back('.');
    }
    floatString.erase(floatString.find_last_not_of('0') + 1,
    std::string::npos); if (floatString.back() == '.') {
      floatString += '0';
    }
    return floatString;
  }
  // JNodeNumeric Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("JNodeNumeric Error: " + message) {}
  };
  // Convert types
  template <typename T> T convertType() const {
    if (m_type == Type::tInt) {
      return (static_cast<T>(m_values.m_integer));
    } else if (m_type == Type::tLong) {
      return (static_cast<T>(m_values.m_long));
    } else if (m_type == Type::tLLong) {
      return (static_cast<T>(m_values.m_llong));
    } else if (m_type == Type::tFloat) {
      return (static_cast<T>(m_values.m_float));
    } else if (m_type == Type::tDouble) {
      return (static_cast<T>(m_values.m_double));
    } else if (m_type == Type::tLDouble) {
      return (static_cast<T>(m_values.m_double));
    }
    throw Error("Could not convert unknown type.");
  }
  // Number type
  enum class Type { tInt = 0, tLong, tLLong, tFloat, tDouble, tLDouble };
  // Numeric union
  union Numbers {
    int m_integer;
    long m_long;
    llong m_llong;
    float m_float;
    double m_double;
    ldouble m_ldouble;
  };
  // Constructors/Destructors
  JNodeNumeric() = default;
  explicit JNodeNumeric(const std::string &number) {
    [[maybe_unused]] auto valid = setValidNumber(number);
  }
  explicit JNodeNumeric(int integer) {
    m_values.m_integer = integer;
    m_type = Type::tInt;
  }
  explicit JNodeNumeric(long integer) {
    m_values.m_long = integer;
    m_type = Type::tLong;
  }
  explicit JNodeNumeric(llong integer) {
    m_values.m_llong = integer;
    m_type = Type::tLLong;
  }
  explicit JNodeNumeric(float floatingPoint) {
    m_values.m_float = floatingPoint;
    m_type = Type::tFloat;
  }
  explicit JNodeNumeric(double floatingPoint) {
    m_values.m_double = floatingPoint;
    m_type = Type::tDouble;
  }
  explicit JNodeNumeric(ldouble floatingPoint) {
    m_values.m_ldouble = floatingPoint;
    m_type = Type::tLDouble;
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
  // Is number a int/long/llong/float/double/ldouble ?
  [[nodiscard]] bool isInt() const { return (m_type == Type::tInt); }
  [[nodiscard]] bool isLong() const { return (m_type == Type::tLong); }
  [[nodiscard]] bool isLLong() const { return (m_type == Type::tLLong); }
  [[nodiscard]] bool isFloat() const { return (m_type == Type::tFloat); }
  [[nodiscard]] bool isDouble() const { return (m_type == Type::tDouble); }
  [[nodiscard]] bool isLDouble() const { return (m_type == Type::tLDouble); }
  // Return numbers value int/long/llong/float/double/ldouble.
  // Note: Can still return a long value for floating point.
  [[nodiscard]] int getInt() const { return (convertType<int>()); }
  [[nodiscard]] long getLong() const { return (convertType<long>()); }
  [[nodiscard]] llong getLLong() const { return (convertType<llong>()); }
  [[nodiscard]] float getFloat() const { return (convertType<float>()); }
  [[nodiscard]] double getDouble() const { return (convertType<double>()); }
  [[nodiscard]] ldouble getLDouble() const { return (convertType<ldouble>()); }
  // Set numbers value to int/long/llong/float/double/ldouble
  // returning true if the value is set.
  [[nodiscard]] bool setInt(const std::string &number) {
    try {
      std::size_t end;
      m_values.m_integer = std::stoi(number.c_str(), &end, 10);
      if (end != number.size()) {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tInt;
    return (true);
  }
  [[nodiscard]] bool setInt(int number) {
    m_type = Type::tInt;
    m_values.m_integer = number;
    return (true);
  }
  [[nodiscard]] bool setLong(const std::string &number) {
    try {
      char *end;
      m_values.m_long = std::strtol(number.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tLong;
    return (true);
  }
  [[nodiscard]] bool setLong(long number) {
    m_type = Type::tLong;
    m_values.m_long = number;
    return (true);
  }
  [[nodiscard]] bool setLLong(const std::string &number) {
    try {
      char *end;
      m_values.m_llong = std::strtoll(number.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tLLong;
    return (true);
  }
  [[nodiscard]] bool setLLong(llong number) {
    m_type = Type::tLLong;
    m_values.m_llong = number;
    return (true);
  }
  [[nodiscard]] bool setFloat(const std::string &number) {
    try {
      std::size_t end;
      m_values.m_float = std::stof(number.c_str(), &end);
      if (end != number.size()) {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tFloat;
    return (true);
  }
  [[nodiscard]] bool setFloat(float number) {
    m_type = Type::tFloat;
    m_values.m_float = number;
    return (true);
  }
  [[nodiscard]] bool setDouble(const std::string &number) {
    try {
      char *end;
      m_values.m_double = std::strtod(number.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tDouble;
    return (true);
  }
  [[nodiscard]] bool setDouble(double number) {
    m_type = Type::tDouble;
    m_values.m_double = number;
    return (true);
  }
  [[nodiscard]] bool setLDouble(const std::string &number) {
    try {
      char *end;
      m_values.m_ldouble = std::strtold(number.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::tLDouble;
    return (true);
  }
  [[nodiscard]] bool setLDouble(ldouble number) {
    m_type = Type::tLDouble;
    m_values.m_ldouble = number;
    return (true);
  }
  // Set numeric value
  [[nodiscard]] bool setValidNumber(const std::string &number) {
    return (setInt(number) || setLong(number) || setLLong(number) ||
            setFloat(number) || setDouble(number) || setLDouble(number));
  }
  // Get string representation of numeric
  [[nodiscard]] std::string getString() const {
    switch (m_type) {
    case Type::tInt:
      return (numericToString(m_values.m_integer));
    case Type::tLong:
      return (numericToString(m_values.m_long));
    case Type::tLLong:
      return (numericToString(m_values.m_llong));
    case Type::tFloat:
      return (numericToString(m_values.m_float));
    case Type::tDouble:
      return (numericToString(m_values.m_double));
    case Type::tLDouble:
      return (numericToString(m_values.m_ldouble));
    }
    throw Error("Could not convert unknown type.");
  }
private:
  Type m_type;
  Numbers m_values;
};
} // namespace JSONLib