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
  // Floating point notation
  enum class Notation { normal = 0, fixed, scientific };
  // Number to string
  template<Integer T> [[nodiscard]] std::string numericToString(const T &number) const
  {
    std::ostringstream os;
    os << number;
    return os.str();
  }
  template<Float T> [[nodiscard]] std::string numericToString(const T &number) const
  {
    std::ostringstream os;
    switch (m_notation) {
    case Notation::normal:
      os << std::defaultfloat << std::setprecision(m_precision) << number;
      break;
    case Notation::fixed:
      os << std::fixed << std::setprecision(m_precision) << number;
      break;
    case Notation::scientific:
      os << std::scientific << std::setprecision(m_precision) << number;
      break;
    default:
      os << std::setprecision(m_precision) << number;
    }
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
  explicit Numeric(int integer) : m_type(Type::Int) { m_values.m_integer = integer; }
  explicit Numeric(long integer) : m_type(Type::Long) { m_values.m_long = integer; }
  explicit Numeric(long long integer) : m_type(Type::LongLong) { m_values.m_longlong = integer; }
  explicit Numeric(float floatingPoint) : m_type(Type::Float) { m_values.m_float = floatingPoint; }
  explicit Numeric(double floatingPoint) : m_type(Type::Double) { m_values.m_double = floatingPoint; }
  explicit Numeric(long double floatingPoint) : m_type(Type::LongDouble) { m_values.m_longdouble = floatingPoint; }
  Numeric(const Numeric &other) = default;
  Numeric &operator=(const Numeric &other) = default;
  Numeric(Numeric &&other) = default;
  Numeric &operator=(Numeric &&other) = default;
  ~Numeric() = default;
  // Is number a int/long/long long/float/double/long double ?
  template<typename T> [[nodiscard]] bool is() const
  {
    if constexpr (std::is_same_v<T, int>) {
      return (m_type == Type::Int);
    } else if constexpr (std::is_same_v<T, long>) {
      return (m_type == Type::Long);
    } else if constexpr (std::is_same_v<T, long long>) {
      return (m_type == Type::LongLong);
    } else if constexpr (std::is_same_v<T, float>) {
      return (m_type == Type::Float);
    } else if constexpr (std::is_same_v<T, double>) {
      return (m_type == Type::Double);
    } else if constexpr (std::is_same_v<T, long double>) {
      return (m_type == Type::LongDouble);
    }
  }
  // Return numbers value int/long long/float/double/long double.
  // Note: Can still return a integer value for a floating point.
  // Number to string
  template<typename T> [[nodiscard]] T get() const { return (convertType<T>()); }
  // Set numbers value to int/long/long long/float/double/long double
  // returning true if the value is set.
  template<typename T> [[nodiscard]] bool set(T number)
  {
    *this = Numeric(number);
    return (true);
  }
  // Set numeric value
  [[nodiscard]] bool setValidNumber(const std::string &number)
  {
    // Find the smallest type that can represent a number. Note: That if it cannot be held as an
    // integer then floating point types are tried.
    return (setInt(number) || setLong(number) || setLongLong(number) || setFloat(number) || setDouble(number)
            || setLongDouble(number));
  }
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
  inline static void setNotation(Notation notation) { m_notation = notation; }

private:
  [[nodiscard]] bool setInt(const std::string &number)
  {
    try {
      std::size_t end = 0;
      int integer = std::stoi(number, &end, kStringConversionBase);
      if (end != number.size()) { return (false); }
      *this = Numeric(integer);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool setLong(const std::string &number)
  {
    try {
      char *end = nullptr;
      errno = 0;
      long integer = std::strtol(number.c_str(), &end, kStringConversionBase);
      if ((*end != '\0') || (errno == ERANGE)) { return (false); }
      *this = Numeric(integer);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool setLongLong(const std::string &number)
  {
    try {
      char *end = nullptr;
      errno = 0;
      long long integer = std::strtoll(number.c_str(), &end, kStringConversionBase);
      if ((*end != '\0') || (errno == ERANGE)) { return (false); }
      *this = Numeric(integer);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool setFloat(const std::string &number)
  {
    try {
      std::size_t end = 0;
      float floatingPoint = std::stof(number, &end);
      if (end != number.size()) { return (false); }
      *this = Numeric(floatingPoint);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool setDouble(const std::string &number)
  {
    try {
      char *end = nullptr;
      double floatingPoint = std::strtod(number.c_str(), &end);
      if (*end != '\0') { return (false); }
      *this = Numeric(floatingPoint);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] bool setLongDouble(const std::string &number)
  {
    try {
      char *end = nullptr;
      long double floatingPoint = std::strtold(number.c_str(), &end);
      if (*end != '\0') { return (false); }
      *this = Numeric(floatingPoint);
    } catch ([[maybe_unused]] const std::exception &e) {
      return (false);
    }
    return (true);
  }
  // Number type
  Type m_type{ Type::Unknown };
  // Number value union
  Numbers m_values{ 0 };
  // Floating point to string parameters
  inline static int m_precision{ 6 };
  inline static Notation m_notation{ Notation::normal };
};
}// namespace JSONLib