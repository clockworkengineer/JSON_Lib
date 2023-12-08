#pragma once

#include <string>

namespace JSON_Lib {
// ======
// String
// ======
struct String : Variant
{
  // Constructors/Destructors
  String() : Variant(Variant::Type::string) {}
  explicit String(std::string string) : Variant(Variant::Type::string), jNodeString(std::move(string)) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &getString() { return (jNodeString); }
  [[nodiscard]] const std::string &getString() const { return (jNodeString); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (jNodeString); }

private:
  std::string jNodeString;
};
}// namespace JSON_Lib