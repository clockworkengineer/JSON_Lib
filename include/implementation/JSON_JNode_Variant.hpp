#pragma once
// =======
// C++ STL
// =======
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// ====
// Base
// ====
struct Variant
{
  // JNode Variant Types
  enum class Type { base = 0, object, array, number, string, boolean, null, hole };
  // Constructors/Destructors
  explicit Variant(Variant::Type nodeType = Variant::Type::base) : m_type(nodeType) {}
  [[nodiscard]] Variant::Type getType() const { return (m_type); }
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;

private:
  Variant::Type m_type;
};
}// namespace JSON_Lib