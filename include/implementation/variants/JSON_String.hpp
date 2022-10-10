#pragma once
// =======
// C++ STL
// =======
#include <string>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ======
// String
// ======
struct String : Variant
{
  // Constructors/Destructors
  String() : Variant(Variant::Type::string) {}
  explicit String(std::string string) : Variant(Variant::Type::string), m_string(std::move(string)) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &getString() { return (m_string); }
  [[nodiscard]] const std::string &getString() const { return (m_string); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (m_string); }

private:
  std::string m_string;
};
}// namespace JSON_Lib