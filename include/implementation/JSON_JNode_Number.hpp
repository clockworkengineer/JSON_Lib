#pragma once
// =======
// C++ STL
// =======
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cerrno>
// #include <variant>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ================
// Number concepts
// ================
template<typename T>
concept Integer = std::is_integral<T>::value;
template<typename T>
concept Float = std::is_floating_point<T>::value;
// ======
// Number
// ======
struct Number : Variant
{
  // Number values variant
  using Values = std::variant<std::monostate, int, long, long long, float, double, long double>;
  // JNode Number Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Number Error: " + message) {}
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
  template<typename T, typename U> [[nodiscard]] T convert(U value) const
  {
    if constexpr (std::is_same_v<T, std::string>) {
      return (numericToString(value));
    } else {
      return (static_cast<T>(value));
    }
  }
  // Convert values to another specified type
  template<typename T> [[nodiscard]] T convertType() const
  {
    if (auto pValue = std::get_if<int>(&m_values)) { return (convert<T>(*pValue)); }
    if (auto pValue = std::get_if<long>(&m_values)) { return (convert<T>(*pValue)); }
    if (auto pValue = std::get_if<long long>(&m_values)) { return (convert<T>(*pValue)); }
    if (auto pValue = std::get_if<float>(&m_values)) { return (convert<T>(*pValue)); }
    if (auto pValue = std::get_if<double>(&m_values)) { return (convert<T>(*pValue)); }
    if (auto pValue = std::get_if<long double>(&m_values)) { return (convert<T>(*pValue)); }
    throw Error("Could not convert unknown type.");
  }
  // Constructors/Destructors
  Number() : Variant(Variant::Type::number) {}
  explicit Number(const std::string &string) : Variant(Variant::Type::number) { convertNumber(string); }
  explicit Number(int value) : Variant(Variant::Type::number), m_values(value) {}
  explicit Number(long value) : Variant(Variant::Type::number), m_values(value) {}
  explicit Number(long long value) : Variant(Variant::Type::number), m_values(value) {}
  explicit Number(float value) : Variant(Variant::Type::number), m_values(value) {}
  explicit Number(double value) : Variant(Variant::Type::number), m_values(value) {}
  explicit Number(long double value) : Variant(Variant::Type::number), m_values(value) {}
  Number(const Number &other) = delete;
  Number &operator=(const Number &other) = delete;
  Number(Number &&other) = default;
  Number &operator=(Number &&other) = default;
  ~Number() = default;
  // Is number a int/long/long long/float/double/long double ?
  template<typename T> [[nodiscard]] bool is() const { return (std::get_if<T>(&m_values) != nullptr); }
  // Return numbers value int/long long/float/double/long double.
  // Note: Can still return a integer value for a floating point.
  template<typename T> [[nodiscard]] T get() const { return (convertType<T>()); }
  // Set numbers value to int/long/long long/float/double/long double
  template<typename T> void set(T number) { *this = Number(number); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (get<std::string>()); }
  // Set floating point to string conversion parameters
  inline static void setPrecision(int precision) { m_precision = precision; }
  inline static void setNotation(Notation notation) { m_notation = notation; }
  // Make Number JNode
  static JNode make(Number &number) { return (JNode{ std::make_unique<Number>(std::move(number)) }); }
  template<typename T> static JNode make(T number) { return (JNode{ std::make_unique<Number>(Number{ number }) }); }

private:
  // Try to convert string to specific numeric type (returns true on success)
  template<typename T> bool stringToNumber(const std::string &number)
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
        *this = Number(value);
      } catch ([[maybe_unused]] const std::exception &ex) {
        return (false);
      }
      return (true);
    }
  }
  // Find the smallest type that can represent a number. Note: That if it cannot be held as an
  // integer then floating point types are tried.
  void convertNumber(const std::string &number)
  {
    [[maybe_unused]] auto ok = stringToNumber<int>(number) || stringToNumber<long>(number)
                               || stringToNumber<long long>(number) || stringToNumber<float>(number)
                               || stringToNumber<double>(number) || stringToNumber<long double>(number);
  }
  // Number values (variant)
  Values m_values;
  // Floating point to string parameters
  inline static int m_precision{ 6 };
  inline static Notation m_notation{ Notation::normal };
};
}// namespace JSON_Lib