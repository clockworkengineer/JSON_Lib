#pragma once
// =======
// C++ STL
// =======
#include <string>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// =======
// Boolean
// =======
struct Boolean : Variant
{
  // Constructors/Destructors
  Boolean() : Variant(Variant::Type::boolean) {}
  explicit Boolean(bool boolean) : Variant(Variant::Type::boolean), m_boolean(boolean) {}
  Boolean(const Boolean &other) = default;
  Boolean &operator=(const Boolean &other) = default;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return reference boolean value
  [[nodiscard]] bool &getBoolean() { return (m_boolean); }
  [[nodiscard]] const bool &getBoolean() const { return (m_boolean); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (m_boolean ? "true" : "false"); }

private:
  bool m_boolean{};
};
}// namespace JSON_Lib