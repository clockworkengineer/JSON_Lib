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
  explicit Variant(JNodeType nodeType = JNodeType::base) : m_nodeType(nodeType) {}
  [[nodiscard]] JNodeType getType() const { return (m_nodeType); }
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;

private:
  JNodeType m_nodeType;
};
}// namespace JSONLib