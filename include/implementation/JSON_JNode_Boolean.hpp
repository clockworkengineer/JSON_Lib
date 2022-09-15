#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// =======
// Boolean
// =======
struct Boolean : Variant
{
  // Constructors/Destructors
  Boolean() : Variant(JNode::Type::boolean) {}
  explicit Boolean(bool boolean) : Variant(JNode::Type::boolean), m_boolean(boolean) {}
  Boolean(const Boolean &other) = delete;
  Boolean &operator=(const Boolean &other) = delete;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return reference boolean value
  [[nodiscard]] bool &getBoolean() { return (m_boolean); }
  [[nodiscard]] const bool &getBoolean() const { return (m_boolean); }
  // Return string representation of boolean value
  [[nodiscard]] std::string toString() const { return (m_boolean ? "true" : "false"); }
  // Make Boolean JNode
  static JNode make(bool boolean) { return (JNode{ std::make_unique<Boolean>(Boolean{ boolean }) }); }

private:
  bool m_boolean{};
};
}// namespace JSON_Lib