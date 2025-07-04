#pragma once

namespace JSON_Lib {

// =======================
// What is Node variant ?
// =======================
template<typename T> bool isA(const Node &jNode)
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
// Node reference converter
// =========================
template<typename T> void checkNode(const Node &jNode)
{
  if constexpr (std::is_same_v<T, String>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not a string."); }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not a number."); }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not an array."); }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not an object."); }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not a boolean."); }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not a null."); }
  }else if constexpr (std::is_same_v<T, Hole>) {
    if (!isA<T>(jNode)) { throw Node::Error("Node not a hole."); }
  }
}
template<typename T> T &JRef(Node &jNode)
{
  checkNode<T>(jNode);
  return static_cast<T &>(jNode.getVariant());
}
template<typename T> const T &JRef(const Node &jNode)
{
  checkNode<T>(jNode);
  return static_cast<const T &>(jNode.getVariant());
}
template<typename T> T &JRef(Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return static_cast<T &>(jNodeEntry.getNode().getVariant());
}
template<typename T> const T &JRef(const Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return static_cast<const T &>(jNodeEntry.getNode().getVariant());
}
}// namespace JSON_Lib