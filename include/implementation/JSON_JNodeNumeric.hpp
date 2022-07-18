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
template <typename T> std::string numericToString(const T &t) {
  std::ostringstream os;
  os << t;
  return os.str();
}
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
  [[nodiscard]] int getInt() const {

    if (m_type == NumberType::tInt) {
      return (static_cast<int>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<int>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<int>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<int>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<int>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<int>(m_values.m_double));
    }
    return (m_values.m_integer);
  }
  [[nodiscard]] long getLong() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<long>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<long>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<long>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<long>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<long>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<long>(m_values.m_double));
    }
    return (m_values.m_long);
  }
  [[nodiscard]] long long getLongLong() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<long long>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<long long>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<long long>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<long long>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<long long>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<long long>(m_values.m_double));
    }
    return (m_values.m_longlong);
  }
  [[nodiscard]] float getFloat() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<float>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<float>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<float>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<float>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<float>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<float>(m_values.m_double));
    }
    return (m_values.m_float);
  }
  [[nodiscard]] double getDouble() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<double>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<double>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<double>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<double>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<double>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<double>(m_values.m_double));
    }
    return (m_values.m_double);
  }
  [[nodiscard]] long double getLongDouble() const {
    if (m_type == NumberType::tInt) {
      return (static_cast<long double>(m_values.m_integer));
    } else if (m_type == NumberType::tLong) {
      return (static_cast<long double>(m_values.m_long));
    } else if (m_type == NumberType::tLongLong) {
      return (static_cast<long double>(m_values.m_longlong));
    } else if (m_type == NumberType::tFloat) {
      return (static_cast<long double>(m_values.m_float));
    } else if (m_type == NumberType::tDouble) {
      return (static_cast<long double>(m_values.m_double));
    } else if (m_type == NumberType::tLongDouble) {
      return (static_cast<long double>(m_values.m_double));
    }
    return (m_values.m_longdouble);
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
    std::string toString;
    switch (m_type) {
    case NumberType::tInt:
      toString = numericToString(m_values.m_integer);
      break;
    case NumberType::tLong:
      toString = numericToString(m_values.m_long);
      break;
    case NumberType::tLongLong:
      toString = numericToString(m_values.m_longlong);
      break;
    case NumberType::tFloat:
      toString = numericToString(m_values.m_float);
      if (toString.find(".") == std::string::npos) {
        toString.push_back('.');
      }
      toString.erase(toString.find_last_not_of('0') + 1, std::string::npos);
      if (toString.back() == '.') {
        toString += '0';
      }
      break;
    case NumberType::tDouble:
      toString = numericToString(m_values.m_double);
      if (toString.find(".") == std::string::npos) {
        toString.push_back('.');
      }
      toString.erase(toString.find_last_not_of('0') + 1, std::string::npos);
      if (toString.back() == '.') {
        toString += '0';
      }
      break;
    case NumberType::tLongDouble:
      toString = numericToString(m_values.m_longdouble);
      if (toString.find(".") == std::string::npos) {
        toString.push_back('.');
      }
      toString.erase(toString.find_last_not_of('0') + 1, std::string::npos);
      if (toString.back() == '.') {
        toString += '0';
      }
      break;
    }
    return (toString);
  }

private:
   NumberType m_type;
  Numbers m_values;
};
} // namespace JSONLib