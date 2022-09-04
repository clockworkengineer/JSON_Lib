#pragma once
// =======
// C++ STL
// =======
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ================
// Numeric concepts
// ================
template<typename T>
concept Integer = std::is_integral<T>::value;
template<typename T>
concept Float = std::is_floating_point<T>::value;
// =============
// JNode Numeric
// =============
struct Numeric
{
  // All string conversions are for base 10
  static constexpr int kStringConversionBase{ 10 };
  // Number to string
  template<Integer T> [[nodiscard]] std::string numericToString(const T &t) const
  {
    std::ostringstream os;
    os << t;
    return os.str();
  }
  template<Float T> [[nodiscard]] std::string numericToString(const T &t) const
  {
    std::ostringstream os;
    os << std::setprecision(m_precision) << t;
    if (os.str().find('.') == std::string::npos) { return (os.str() + ".0"); }
    return (os.str());
  }
  // JNode Numeric Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Numeric Error: " + message) {}
  };
  // Convert types
  template<typename T> [[nodiscard]] T convertType() const
  {
    if (m_type == Type::Int) { return (static_cast<T>(m_values.m_integer)); }
    if (m_type == Type::Long) { return (static_cast<T>(m_values.m_long)); }
    if (m_type == Type::LongLong) { return (static_cast<T>(m_values.m_longlong)); }
    if (m_type == Type::Float) { return (static_cast<T>(m_values.m_float)); }
    if (m_type == Type::Double) { return (static_cast<T>(m_values.m_double)); }
    if (m_type == Type::LongDouble) { return (static_cast<T>(m_values.m_longdouble)); }
    throw Error("Could not convert unknown type.");
  }
  // Numeric types
  enum class Type { Unknown = 0, Int, Long, LongLong, Float, Double, LongDouble };
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
  Numeric() = default;
  explicit Numeric(const std::string &number) { [[maybe_unused]] auto valid = setValidNumber(number); }
  explicit Numeric(int integer)
  {
    m_values.m_integer = integer;
    m_type = Type::Int;
  }
  explicit Numeric(long integer)
  {
    m_values.m_long = integer;
    m_type = Type::Long;
  }
  explicit Numeric(long long integer)
  {
    m_values.m_longlong = integer;
    m_type = Type::LongLong;
  }
  explicit Numeric(float floatingPoint)
  {
    m_values.m_float = floatingPoint;
    m_type = Type::Float;
  }
  explicit Numeric(double floatingPoint)
  {
    m_values.m_double = floatingPoint;
    m_type = Type::Double;
  }
  explicit Numeric(long double floatingPoint)
  {
    m_values.m_longdouble = floatingPoint;
    m_type = Type::LongDouble;
  }
  Numeric(const Numeric &other) = default;
  Numeric &operator=(const Numeric &other) = default;
  Numeric(Numeric &&other) = default;
  Numeric &operator=(Numeric &&other) = default;
  ~Numeric() = default;
  // Is number a int/long/long long/float/double/long double ?
  [[nodiscard]] bool isInt() const { return (m_type == Type::Int); }
  [[nodiscard]] bool isLong() const { return (m_type == Type::Long); }
  [[nodiscard]] bool isLongLong() const { return (m_type == Type::LongLong); }
  [[nodiscard]] bool isFloat() const { return (m_type == Type::Float); }
  [[nodiscard]] bool isDouble() const { return (m_type == Type::Double); }
  [[nodiscard]] bool isLongDouble() const { return (m_type == Type::LongDouble); }
  // Return numbers value int/long long/float/double/long double.
  // Note: Can still return a integer value for a floating point.
  [[nodiscard]] int getInt() const { return (convertType<int>()); }
  [[nodiscard]] long getLong() const { return (convertType<long>()); }
  [[nodiscard]] long long getLongLong() const { return (convertType<long long>()); }
  [[nodiscard]] float getFloat() const { return (convertType<float>()); }
  [[nodiscard]] double getDouble() const { return (convertType<double>()); }
  [[nodiscard]] long double getLongDouble() const { return (convertType<long double>()); }
  // Set numbers value to int/long/long long/float/double/long double
  // returning true if the value is set.
  [[nodiscard]] bool setInt(const std::string &number)
  {
    try {
      std::size_t end = 0;
      m_values.m_integer = std::stoi(number, &end, kStringConversionBase);
      if (end != number.size()) { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::Int;
    return (true);
  }
  [[nodiscard]] bool setInt(int number)
  {
    m_type = Type::Int;
    m_values.m_integer = number;
    return (true);
  }
  [[nodiscard]] bool setLong(const std::string &number)
  {
    try {
      char *end = nullptr;
      errno = 0;
      m_values.m_long = std::strtol(number.c_str(), &end, kStringConversionBase);
      if ((*end != '\0') || (errno == ERANGE)) { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::Long;
    return (true);
  }
  [[nodiscard]] bool setLong(long number)
  {
    m_type = Type::Long;
    m_values.m_long = number;
    return (true);
  }
  [[nodiscard]] bool setLongLong(const std::string &number)
  {
    try {
      char *end = nullptr;
      errno = 0;
      m_values.m_longlong = std::strtoll(number.c_str(), &end, kStringConversionBase);
      if ((*end != '\0') || (errno == ERANGE)) { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::LongLong;
    return (true);
  }
  [[nodiscard]] bool setLongLong(long long number)
  {
    m_type = Type::LongLong;
    m_values.m_longlong = number;
    return (true);
  }
  [[nodiscard]] bool setFloat(const std::string &number)
  {
    try {
      std::size_t end = 0;
      m_values.m_float = std::stof(number, &end);
      if (end != number.size()) { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::Float;
    return (true);
  }
  [[nodiscard]] bool setFloat(float number)
  {
    m_type = Type::Float;
    m_values.m_float = number;
    return (true);
  }
  [[nodiscard]] bool setDouble(const std::string &number)
  {
    try {
      char *end = nullptr;
      m_values.m_double = std::strtod(number.c_str(), &end);
      if (*end != '\0') { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::Double;
    return (true);
  }
  [[nodiscard]] bool setDouble(double number)
  {
    m_type = Type::Double;
    m_values.m_double = number;
    return (true);
  }
  [[nodiscard]] bool setLongDouble(const std::string &number)
  {
    try {
      char *end = nullptr;
      m_values.m_longdouble = std::strtold(number.c_str(), &end);
      if (*end != '\0') { return (false); }
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    m_type = Type::LongDouble;
    return (true);
  }
  [[nodiscard]] bool setLongDouble(long double number)
  {
    m_type = Type::LongDouble;
    m_values.m_longdouble = number;
    return (true);
  }
  // Set numeric value
  [[nodiscard]] bool setValidNumber(const std::string &number)
  {
    return (setInt(number) || setLong(number) || setLongLong(number) || setFloat(number) || setDouble(number)
            || setLongDouble(number));
  }
  // Get string representation of numeric
  [[nodiscard]] std::string getString() const
  {
    if (m_type == Type::Int) { return (numericToString(m_values.m_integer)); }
    if (m_type == Type::Long) { return (numericToString(m_values.m_long)); }
    if (m_type == Type::LongLong) { return (numericToString(m_values.m_longlong)); }
    if (m_type == Type::Float) { return (numericToString(m_values.m_float)); }
    if (m_type == Type::Double) { return (numericToString(m_values.m_double)); }
    if (m_type == Type::LongDouble) { return (numericToString(m_values.m_longdouble)); }
    throw Error("Could not convert unknown type.");
  }
  // Set floating point to string conversion parameters
  inline static void setPrecision(int precision) { m_precision = precision; }

private:
  // Number type
  Type m_type{ Type::Unknown };
  // Number value union
  Numbers m_values{ 0 };
  // Floating point to string parameters
  inline static int m_precision{ 6 };
};
}// namespace JSONLib