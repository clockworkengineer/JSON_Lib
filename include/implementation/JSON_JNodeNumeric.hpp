#pragma once
// =======
// C++ STL
// =======
#include <sstream>
#include <string>
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
    floatString.erase(floatString.find_last_not_of('0') + 1, std::string::npos);
    if (floatString.back() == '.') {
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
  template <typename T> T convert() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<T>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<T>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<T>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<T>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<T>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<T>(m_values.m_double));
    }
    throw Error("Could not convert unknown type.");
  }
  // Number type
  enum class NumberType {
    tInt = 0,
    tLong,
    tLongLong,
    tFloat,
    tDouble,
    tLongDouble
  };
  // Numeric union
  union Numbers {
    int m_integer;
    long m_long;
    long long m_longlong;
    float m_float;
    double m_double;
    long double m_longdouble;
  };
  // Constructors/Destructors
  JNodeNumeric() = default;
  explicit JNodeNumeric(const std::string &number) {
    [[maybe_unused]] auto valid = setValidNumber(number);
  }
  explicit JNodeNumeric(int integer) {
    m_values.m_integer = integer;
    m_type = NumberType::tInt;
  }
  explicit JNodeNumeric(long integer) {
    m_values.m_long = integer;
    m_type = NumberType::tLong;
  }
  explicit JNodeNumeric(long long integer) {
    m_values.m_longlong = integer;
    m_type = NumberType::tLongLong;
  }
  explicit JNodeNumeric(float floatingPoint) {
    m_values.m_float = floatingPoint;
    m_type = NumberType::tFloat;
  }
  explicit JNodeNumeric(double floatingPoint) {
    m_values.m_double = floatingPoint;
    m_type = NumberType::tDouble;
  }
  explicit JNodeNumeric(long double floatingPoint) {
    m_values.m_longdouble = floatingPoint;
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
  [[nodiscard]] bool isInt() const { return (m_type == NumberType::tInt); }
  [[nodiscard]] bool isLong() const { return (m_type == NumberType::tLong); }
  [[nodiscard]] bool isLongLong() const {
    return (m_type == NumberType::tLongLong);
  }
  [[nodiscard]] bool isFloat() const { return (m_type == NumberType::tFloat); }
  [[nodiscard]] bool isDouble() const {
    return (m_type == NumberType::tDouble);
  }
  [[nodiscard]] bool isLongDouble() const {
    return (m_type == NumberType::tLongDouble);
  }
  // Return numbers value int/long/long long/float/double/long double.
  // Note: Can still return a long value for floating point.
  template <typename T> [[nodiscard]] auto  get() const { return (convert<auto>()); }
  [[nodiscard]] int getInt() const { return (convert<int>()); }
  [[nodiscard]] long getLong() const { return (convert<long>()); }
  [[nodiscard]] long long getLongLong() const { return (convert<long long>()); }
  [[nodiscard]] float getFloat() const { return (convert<float>()); }
  [[nodiscard]] double getDouble() const { return (convert<double>()); }
  [[nodiscard]] long double getLongDouble() const {
    return (convert<long double>());
  }
  // Set numbers value to int/long/long long/float/double/long double
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
    m_type = NumberType::tInt;
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
    m_type = NumberType::tLong;
    return (true);
  }
  [[nodiscard]] bool setLongLong(const std::string &number) {
    try {
      char *end;
      m_values.m_longlong = std::strtoll(number.c_str(), &end, 10);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tLongLong;
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
    m_type = NumberType::tFloat;
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
    m_type = NumberType::tDouble;
    return (true);
  }
  [[nodiscard]] bool setLongDouble(const std::string &number) {
    try {
      char *end;
      m_values.m_longdouble = std::strtold(number.c_str(), &end);
      if (*end != '\0') {
        return (false);
      }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = NumberType::tLongDouble;
    return (true);
  }
  // Set numeric value
  [[nodiscard]] bool setValidNumber(const std::string &number) {
    if (setInt(number)) {
      return (true);
    }
    if (setLong(number)) {
      return (true);
    }
    if (setLongLong(number)) {
      return (true);
    }
    if (setFloat(number)) {
      return (true);
    }
    if (setDouble(number)) {
      return (true);
    }
    if (setLongDouble(number)) {
      return (true);
    }
    return (false);
  }
  // Get string representation of numeric
  [[nodiscard]] std::string getString() const {
    switch (m_type) {
    case NumberType::tInt:
      return (numericToString(m_values.m_integer));
    case NumberType::tLong:
      return (numericToString(m_values.m_long));
    case NumberType::tLongLong:
      return (numericToString(m_values.m_longlong));
    case NumberType::tFloat:
      return (numericToString(m_values.m_float));
    case NumberType::tDouble:
      return (numericToString(m_values.m_double));
    case NumberType::tLongDouble:
      return (numericToString(m_values.m_longdouble));
    }
    throw Error("Could not convert unknown type.");
  }

private:
  NumberType m_type;
  Numbers m_values;
};
} // namespace JSONLib