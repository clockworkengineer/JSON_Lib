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
  // Numeric variant
  using Numbers = std::variant<std::monostate, int, long, long long, float, double, long double>;
  // JNode Numeric Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Numeric Error: " + message) {}
  };
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
  // Convert values to another specified type
  template<typename T> [[nodiscard]] T convertType() const
  {
    if (auto pInteger = std::get_if<int>(&m_values)) {
      return (static_cast<T>(*pInteger));
    } else if (auto pLong = std::get_if<long>(&m_values)) {
      return (static_cast<T>(*pLong));
    } else if (auto pLongLong = std::get_if<long long>(&m_values)) {
      return (static_cast<T>(*pLongLong));
    } else if (auto pFloat = std::get_if<float>(&m_values)) {
      return (static_cast<T>(*pFloat));
    } else if (auto pDouble = std::get_if<double>(&m_values)) {
      return (static_cast<T>(*pDouble));
    } else if (auto pLongDouble = std::get_if<long double>(&m_values)) {
      return (static_cast<T>(*pLongDouble));
    }
    throw Error("Could not convert unknown type.");
  }
  // Constructors/Destructors
  Numeric() = default;
  explicit Numeric(const std::string &number) { convertNumber(number); }
  explicit Numeric(int integer) : m_values(integer) {}
  explicit Numeric(long integer) : m_values(integer) {}
  explicit Numeric(long long integer) : m_values(integer) {}
  explicit Numeric(float floatingPoint) : m_values(floatingPoint) {}
  explicit Numeric(double floatingPoint) : m_values(floatingPoint) {}
  explicit Numeric(long double floatingPoint) : m_values(floatingPoint) {}
  Numeric(const Numeric &other) = default;
  Numeric &operator=(const Numeric &other) = default;
  Numeric(Numeric &&other) = default;
  Numeric &operator=(Numeric &&other) = default;
  ~Numeric() = default;
  // Is number a int/long/long long/float/double/long double ?
  template<typename T> [[nodiscard]] bool is() const { return (std::get_if<T>(&m_values) != nullptr); }
  // Return numbers value int/long long/float/double/long double.
  // Note: Can still return a integer value for a floating point.
  template<typename T> [[nodiscard]] T get() const { return (convertType<T>()); }
  // Set numbers value to int/long/long long/float/double/long double
  template<typename T> [[nodiscard]] void set(T number) { *this = Numeric(number); }
  // Get numeric string value
  [[nodiscard]] std::string getString() const
  {
    if (auto pInteger = std::get_if<int>(&m_values)) {
      return (numericToString(*pInteger));
    } else if (auto pLong = std::get_if<long>(&m_values)) {
      return (numericToString(*pLong));
    } else if (auto pLongLong = std::get_if<long long>(&m_values)) {
      return (numericToString(*pLongLong));
    } else if (auto pFloat = std::get_if<float>(&m_values)) {
      return (numericToString(*pFloat));
    } else if (auto pDouble = std::get_if<double>(&m_values)) {
      return (numericToString(*pDouble));
    } else if (auto pLongDouble = std::get_if<long double>(&m_values)) {
      return (numericToString(*pLongDouble));
    }
    throw Error("Could not convert unknown type.");
  }
  // Set floating point to string conversion parameters
  inline static void setPrecision(int precision) { m_precision = precision; }
  inline static void setNotation(Notation notation) { m_notation = notation; }

private:
  // Try to convert string to specific numeric type (returns true on success)
  template<typename T> bool stringToNumeric(const std::string &number)
  {
    {
      try {
        std::size_t end = 0;
        T value;
        if constexpr (std::is_same_v<T, int>) {
          value = std::stoi(number, &end, kStringConversionBase);
        } else if constexpr (std::is_same_v<T, long>) {
          value = std::stol(number, &end, kStringConversionBase);
        } else if constexpr (std::is_same_v<T, long long>) {
          value = std::stoll(number, &end, kStringConversionBase);
        } else if constexpr (std::is_same_v<T, float>) {
          value = std::stof(number, &end);
        } else if constexpr (std::is_same_v<T, double>) {
          value = std::stod(number, &end);
        } else if constexpr (std::is_same_v<T, long double>) {
          value = std::stold(number, &end);
        }
        if (end != number.size()) { return (false); }
        *this = Numeric(value);
      } catch ([[maybe_unused]] const std::exception &e) {
        return (false);
      }
      return (true);
    }
  }
  // Find the smallest type that can represent a number. Note: That if it cannot be held as an
  // integer then floating point types are tried.
  void convertNumber(const std::string &number)
  {
    [[maybe_unused]] auto ok = stringToNumeric<int>(number) || stringToNumeric<long>(number)
                               || stringToNumeric<long long>(number) || stringToNumeric<float>(number)
                               || stringToNumeric<double>(number) || stringToNumeric<long double>(number);
  }
  // Numeric variants
  Numbers m_values;
  // Floating point to string parameters
  inline static int m_precision{ 6 };
  inline static Notation m_notation{ Notation::normal };
};
}// namespace JSONLib