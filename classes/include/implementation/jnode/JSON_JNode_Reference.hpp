#pragma once

namespace JSON_Lib {

// =========================
// JNode reference converter
// =========================
template<typename T> void checkJNode(const JNode &jNode)
{
  if constexpr (std::is_same_v<T, String>) {
    if (!jNode.isString()) { throw JNode::Error("JNode not a string."); }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (!jNode.isNumber()) { throw JNode::Error("JNode not a number."); }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (!jNode.isArray()) { throw JNode::Error("JNode not an array."); }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (!jNode.isObject()) { throw JNode::Error("JNode not an object."); }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (!jNode.isBoolean()) { throw JNode::Error("JNode not a boolean."); }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (!jNode.isNull()) { throw JNode::Error("JNode not a null."); }
  }
}
template<typename T> T &JRef(JNode &jNode)
{
  checkJNode<T>(jNode);
  return static_cast<T &>(jNode.getVariant());
}
template<typename T> const T &JRef(const JNode &jNode)
{
  checkJNode<T>(jNode);
  return static_cast<const T &>(jNode.getVariant());
}
template<typename T> T &JRef(Object::Entry &jNodeEntry)
{
  checkJNode<T>(jNodeEntry.getJNode());
  return static_cast<T &>(jNodeEntry.getJNode().getVariant());
}
template<typename T> const T &JRef(const Object::Entry &jNodeEntry)
{
  checkJNode<T>(jNodeEntry.getJNode());
  return static_cast<const T &>(jNodeEntry.getJNode().getVariant());
}
}// namespace JSON_Lib