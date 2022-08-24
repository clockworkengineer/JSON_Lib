#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <unordered_map>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ======
// String
// ======
struct String : Variant
{
  // Constructors/Destructors
  String() : Variant(JNodeType::string) {}
  explicit String(std::string string) : Variant(JNodeType::string), m_jsonString(std::move(string)) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &getString() { return (m_jsonString); }
  [[nodiscard]] const std::string &getString() const { return (m_jsonString); }
  // Convert string representation to a string
  [[nodiscard]] std::string toString() const { return (m_jsonString); }
  // Make String JNode
  static JNode make(const std::string &string) { return (JNode{ std::make_unique<String>(String{ string }) }); }

private:
  std::string m_jsonString;
};
}// namespace JSONLib