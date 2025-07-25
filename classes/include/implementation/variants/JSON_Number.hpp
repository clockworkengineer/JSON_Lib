#pragma once

namespace JSON_Lib {

struct Number : Variant
{
  // Number values variant
  using Values = std::variant<std::monostate, int, long, long long, float, double, long double>;
  // All string conversions are for base 10
  static constexpr int kStringConversionBase{ 10 };
  // Floating point notation
  enum class numberNotation { normal = 0, fixed, scientific };
  // Constructors/Destructors
  Number() : Variant(Type::number) {}
  template<typename T> explicit Number(T value);
  Number(const Number &other) = default;
  Number &operator=(const Number &other) = default;
  Number(Number &&other) = default;
  Number &operator=(Number &&other) = default;
  ~Number() = default;
  // Is number a int/long/long long/float/double/long double ?
  template<typename T> [[nodiscard]] bool is() const { return std::get_if<T>(&jNodeNumber) != nullptr; }
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
    [[maybe_unused]] auto ok = stringToNumber<int>(number) || stringToNumber<long>(number)
                               || stringToNumber<long long>(number) || stringToNumber<float>(number)
                               || stringToNumber<double>(number) || stringToNumber<long double>(number);
  }
  // Number values (variant)
  Values jNodeNumber;
  // Floating point to string parameters
  inline static int numberPrecision{ 6 };
  inline static auto numberNotation{ numberNotation::normal };
};
// Construct Number from value
template<typename T> Number::Number(T value) : Variant(Type::number)
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
  {
    try {
      std::size_t end = 0;
      T value;
      if constexpr (std::is_same_v<T, int>) {
        value = std::stoi(number.data(), &end, kStringConversionBase);
      } else if constexpr (std::is_same_v<T, long>) {
        value = std::stol(number.data(), &end, kStringConversionBase);
      } else if constexpr (std::is_same_v<T, long long>) {
        value = std::stoll(number.data(), &end, kStringConversionBase);
      } else if constexpr (std::is_same_v<T, float>) {
        value = std::stof(number.data(), &end);
      } else if constexpr (std::is_same_v<T, double>) {
        value = std::stod(number.data(), &end);
      } else if constexpr (std::is_same_v<T, long double>) {
        value = std::stold(number.data(), &end);
      }
      if (end != number.size()) { return false; }
      *this = Number(value);
    } catch ([[maybe_unused]] const std::exception &ex) {
      return false;
    }
    return true;
  }
}
// Number to string
template<typename T> std::string Number::numberToString(const T &number) const
{
  std::ostringstream os;
  if constexpr (std::is_floating_point_v<T>) {
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
  } else {
    os << number;
  }
  return os.str();
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
  if (const auto pValue = std::get_if<int>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  if (const auto pValue = std::get_if<long>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  if (const auto pValue = std::get_if<long long>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  if (const auto pValue = std::get_if<float>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  if (const auto pValue = std::get_if<double>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  if (const auto pValue = std::get_if<long double>(&jNodeNumber)) { return convertTo<T>(*pValue); }
  throw Node::Error("Could not convert unknown type.");
}
}// namespace JSON_Lib