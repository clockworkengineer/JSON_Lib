#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// ======
// String
// ======
struct String : Variant
{
  // Constructors/Destructors
  String() : Variant(JNode::Type::string) {}
  explicit String(std::string string) : Variant(JNode::Type::string), m_string(std::move(string)) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &getString() { return (m_string); }
  [[nodiscard]] const std::string &getString() const { return (m_string); }
  // Convert string representation to a string
  [[nodiscard]] std::string toString() const { return (m_string); }
  // Make String JNode
  static JNode make(const std::string &string) { return (JNode{ std::make_unique<String>(String{ string }) }); }

private:
  std::string m_string;
};
}// namespace JSON_Lib