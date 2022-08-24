#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =======
// Boolean
// =======
struct Boolean : Variant
{
  // Constructors/Destructors
  Boolean() : Variant(JNodeType::boolean) {}
  explicit Boolean(bool boolean) : Variant(JNodeType::boolean), m_jsonBoolean(boolean) {}
  Boolean(const Boolean &other) = delete;
  Boolean &operator=(const Boolean &other) = delete;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return reference boolean value
  [[nodiscard]] bool &getBoolean() { return (m_jsonBoolean); }
  [[nodiscard]] const bool &getBoolean() const { return (m_jsonBoolean); }
  // Return string representation of boolean value
  [[nodiscard]] std::string toString() const { return (m_jsonBoolean ? "true" : "false"); }
  // Make Boolean JNode
  static JNode make(bool boolean) { return (JNode{ std::make_unique<Boolean>(Boolean{ boolean }) }); }

private:
  bool m_jsonBoolean{};
};
}// namespace JSONLib