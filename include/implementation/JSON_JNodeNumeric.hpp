#pragma once
// =======
// C++ STL
// =======
#include <array>
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==========
// JSON Error
// ==========
// ============
// JNode Number
// ============
struct JNodeNumeric {
  JNodeNumeric() = default;
  explicit JNodeNumeric(int integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long long integer) {
    std::string number{std::to_string(integer)};
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(float floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(double floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  explicit JNodeNumeric(long double floatingPoint) {
    std::string number{std::to_string(floatingPoint)};
    number.erase(number.find_last_not_of('0') + 1, std::string::npos);
    if (number.back() == '.') {
      number += '0';
    }
    std::memcpy(&value[0], number.c_str(), number.size() + 1);
  }
  JNodeNumeric(const JNodeNumeric &other) = default;
  JNodeNumeric &operator=(const JNodeNumeric &other) = default;
  JNodeNumeric(JNodeNumeric &&other) = default;
  JNodeNumeric &operator=(JNodeNumeric &&other) = default;
  ~JNodeNumeric() = default;
  // Numeric values max width in characters
  static constexpr int kLongLongWidth =
      std::numeric_limits<long long>::digits10 + 2;
  static constexpr int kLongDoubleWidth =
      std::numeric_limits<long double>::digits10 + 2;
  // Numbers stored in fix length array
  std::array<char, kLongLongWidth> value{};
};
} // namespace JSONLib