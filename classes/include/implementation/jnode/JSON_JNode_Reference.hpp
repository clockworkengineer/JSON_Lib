#pragma once

namespace JSON_Lib {

// =======================
// What is JNode variant ?
// =======================
template<typename T> bool isA(const JNode &jNode)
{
  if constexpr (std::is_same_v<T, String>) {
    return jNode.getVariant().getNodeType() == Variant::Type::string;
  } else if constexpr (std::is_same_v<T, Number>) {
    return jNode.getVariant().getNodeType() == Variant::Type::number;
  } else if constexpr (std::is_same_v<T, Array>) {
    return jNode.getVariant().getNodeType() == Variant::Type::array;
  } else if constexpr (std::is_same_v<T, Object>) {
    return jNode.getVariant().getNodeType() == Variant::Type::object;
  } else if constexpr (std::is_same_v<T, Boolean>) {
    return jNode.getVariant().getNodeType() == Variant::Type::boolean;
  } else if constexpr (std::is_same_v<T, Null>) {
    return jNode.getVariant().getNodeType() == Variant::Type::null;
  } else if constexpr (std::is_same_v<T, Hole>) {
    return jNode.getVariant().getNodeType() == Variant::Type::hole;
  } else {
    return false;
  }
}

// =========================
// JNode reference converter
// =========================
template<typename T> void checkJNode(const JNode &jNode)
{
  if constexpr (std::is_same_v<T, String>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not a string."); }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not a number."); }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not an array."); }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not an object."); }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not a boolean."); }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not a null."); }
  }else if constexpr (std::is_same_v<T, Hole>) {
    if (!isA<T>(jNode)) { throw JNode::Error("JNode not a hole."); }
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