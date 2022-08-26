#pragma once
// =======
// C++ STL
// =======
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==============
// JNode variants
// ==============
// ====
// Base
// ====
struct Variant
{
  explicit Variant(JNode::Type nodeType = JNode::Type::base) : m_nodeType(nodeType) {}
  [[nodiscard]] JNode::Type getType() const { return (m_nodeType); }
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;

private:
  JNode::Type m_nodeType;
};
}// namespace JSONLib