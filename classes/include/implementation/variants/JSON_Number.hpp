#pragma once
#include "JSON_Throw.hpp"

#include "JSON_Config.hpp"
#include <charconv>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#if JSON_LIB_EMBEDDED
#include <array>
#else
#include <iomanip>
#include <sstream>
#endif

namespace JSON_Lib {

struct Number
{
  // Number values variant
  using Values = std::variant<std::monostate, int, long, long long, float, double, long double>;
  // All string conversions are for base 10
  static constexpr int kStringConversionBase{ 10 };
  // Floating point notation
  enum class numberNotation { normal = 0, fixed, scientific };
  // Constructors/Destructors
  Number() = default;
  explicit Number(const std::string_view &value) { convertNumber(value); }
  template<typename T> explicit Number(T value);
  Number(const Number &other) = default;
  Number &operator=(const Number &other) = default;
  Number(Number &&other) = default;
  Number &operator=(Number &&other) = default;
  ~Number() = default;
  // Is number a int/long/long long/float/double/long double ?
  template<typename T> [[nodiscard]] bool is() const { return std::get_if<T>(&jNodeNumber) != nullptr; }
  // Did the number parse successfully (i.e. variant is not monostate)?
  [[nodiscard]] bool isValid() const noexcept { return !std::holds_alternative<std::monostate>(jNodeNumber); }
  // Return numbers value int/long long/float/double/long double.
  // Note: Can still return a integer value for a floating point.
  template<typename T> [[nodiscard]] T value() const { return getAs<T>(); }
  // Set numbers value to int/long/long long/float/double/long double
  template<typename T> void set(T number) { *this = Number(number); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return getAs<std::string>(); }
  // Set floating point to string conversion parameters
   static void setPrecision(const int precision) { numberPrecision = precision; }
   static void setNotation(const numberNotation notation) { numberNotation = notation; }

private:
  // Convert string to specific numeric type (returns true on success)
  template<typename T> bool stringToNumber(const std::string_view &number);
  // Number to string
  template<typename T> [[nodiscard]] std::string numberToString(const T &number) const;
  // Convert values to another specified type
  template<typename T, typename U> [[nodiscard]] T convertTo(U value) const;
  // Convert values to another specified type
  template<typename T> [[nodiscard]] T getAs() const;
  // Find the smallest type that can represent a number. Note: That if it cannot be held as an
  // integer then floating point types are tried.
  void convertNumber(const std::string_view &number)
  {
    stringToNumber<int>(number) || stringToNumber<long>(number) || stringToNumber<long long>(number)
      || stringToNumber<float>(number) || stringToNumber<double>(number) || stringToNumber<long double>(number);
  }
  // Number values (variant)
  Values jNodeNumber;
  // Floating point to string parameters
  inline static int numberPrecision{ 6 };
  inline static auto numberNotation{ numberNotation::normal };
};
// Construct Number from value
template<typename T> Number::Number(T value)
{
  if constexpr (std::is_same_v<T, std::string>) {
    convertNumber(value);
  } else {
    jNodeNumber = value;
  }
}
// Convert string to specific numeric type (returns true on success)
template<typename T> bool Number::stringToNumber(const std::string_view &number)
{
  if constexpr (std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, long long>) {
    T value{};
    const auto result = std::from_chars(number.data(), number.data() + number.size(), value, kStringConversionBase);
    if (result.ec != std::errc() || result.ptr != number.data() + number.size()) { return false; }
    *this = Number(value);
    return true;
  } else if constexpr (std::is_floating_point_v<T>) {
    try {
      std::size_t end = 0;
      T value{};
      if constexpr (std::is_same_v<T, float>) {
        value = std::stof(number.data(), &end);
      } else if constexpr (std::is_same_v<T, double>) {
        value = std::stod(number.data(), &end);
      } else {
        value = std::stold(number.data(), &end);
      }
      if (end != number.size()) { return false; }
      *this = Number(value);
      return true;
    } catch (const std::exception &) {
      return false;
    }
  } else {
    static_assert(std::is_same_v<T, void>, "Unsupported numeric type.");
  }
}
// Number to string
template<typename T> std::string Number::numberToString(const T &number) const
{
  if constexpr (std::is_floating_point_v<T>) {
#if JSON_LIB_EMBEDDED
    std::array<char, 64> buf{};
    std::chars_format fmt = std::chars_format::general;
    if (numberNotation == numberNotation::fixed)       fmt = std::chars_format::fixed;
    else if (numberNotation == numberNotation::scientific) fmt = std::chars_format::scientific;
    const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), number, fmt, numberPrecision);
    std::string result(buf.data(), ptr);
    if (result.find('.') == std::string::npos) { result += ".0"; }
    return result;
#else
    std::ostringstream os;
    switch (numberNotation) {
    case numberNotation::normal:
      os << std::defaultfloat << std::setprecision(numberPrecision) << number;
      break;
    case numberNotation::fixed:
      os << std::fixed << std::setprecision(numberPrecision) << number;
      break;
    case numberNotation::scientific:
      os << std::scientific << std::setprecision(numberPrecision) << number;
      break;
    default:
      os << std::setprecision(numberPrecision) << number;
    }
    if (os.str().find('.') == std::string::npos) { return os.str() + ".0"; }
    return os.str();
#endif
  } else {
#if JSON_LIB_EMBEDDED
    std::array<char, 32> buf{};
    const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), number);
    return std::string(buf.data(), ptr);
#else
    std::ostringstream os;
    os << number;
    return os.str();
#endif
  }
}
// Convert value to another specified type
template<typename T, typename U> T Number::convertTo(U value) const
{
  if constexpr (std::is_same_v<T, std::string>) {
    return numberToString(value);
  } else {
    return static_cast<T>(value);
  }
}
// Convert stored number to another specified type
template<typename T> T Number::getAs() const
{
  return std::visit([&](const auto &v) -> T {
    using V = std::decay_t<decltype(v)>;
    if constexpr (std::is_same_v<V, std::monostate>) {
      JSON_THROW(std::runtime_error("Number Error: Could not convert unknown type."));
    } else {
      return convertTo<T>(v);
    }
  }, jNodeNumber);
}
}// namespace JSON_Lib