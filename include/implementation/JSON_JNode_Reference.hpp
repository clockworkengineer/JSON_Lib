#pragma once
// =======
// C++ STL
// =======
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =========================
// JNode reference converter
// =========================
template<typename T> void checkJNode(const Variant &jNodeVariant)
{
  if constexpr (std::is_same_v<T, String>) {
    if (jNodeVariant.getType() != JNode::Type::string) { throw JNode::Error("JNode not a string."); }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (jNodeVariant.getType() != JNode::Type::number) { throw JNode::Error("JNode not a number."); }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (jNodeVariant.getType() != JNode::Type::array) { throw JNode::Error("JNode not an array."); }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (jNodeVariant.getType() != JNode::Type::object) { throw JNode::Error("JNode not an object."); }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (jNodeVariant.getType() != JNode::Type::boolean) { throw JNode::Error("JNode not a boolean."); }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (jNodeVariant.getType() != JNode::Type::null) { throw JNode::Error("JNode not a null."); }
  }
}
template<typename T> T &JRef(JNode &jNode)
{
  checkJNode<T>(jNode.getVariant());
  return (static_cast<T &>(jNode.getVariant()));
}
template<typename T> const T &JRef(const JNode &jNode)
{
  checkJNode<T>(jNode.getVariant());
  return (static_cast<const T &>(jNode.getVariant()));
}
template<typename T> T &JRef(Object::Entry &jNodeEntry)
{
  checkJNode<T>(jNodeEntry.getJNode().getVariant());
  return (static_cast<T &>(jNodeEntry.getJNode().getVariant()));
}
template<typename T> const T &JRef(const Object::Entry &jNodeEntry)
{
  checkJNode<T>(jNodeEntry.getJNode().getVariant());
  return (static_cast<const T &>(jNodeEntry.getJNode().getVariant()));
}
}// namespace JSONLib