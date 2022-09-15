#pragma once
// =======
// C++ STL
// =======
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// ==============
// JNode variants
// ==============
// ====
// Base
// ====
struct Variant
{
  explicit Variant(JNode::Type nodeType = JNode::Type::base) : m_type(nodeType) {}
  [[nodiscard]] JNode::Type getType() const { return (m_type); }
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;

private:
  JNode::Type m_type;
};
}// namespace JSON_Lib