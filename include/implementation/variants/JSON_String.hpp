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
  explicit String(std::string string) : Variant(Variant::Type::string), string(std::move(string)) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &getString() { return (string); }
  [[nodiscard]] const std::string &getString() const { return (string); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (string); }

private:
  std::string string;
};
}// namespace JSON_Lib