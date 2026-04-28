#pragma once

#include <type_traits>

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
  if (!isA<T>(jNode)) {
    if constexpr (std::is_same_v<T, String>) {
      throw Node::Error("Node not a string.");
    } else if constexpr (std::is_same_v<T, Number>) {
      throw Node::Error("Node not a number.");
    } else if constexpr (std::is_same_v<T, Array>) {
      throw Node::Error("Node not an array.");
    } else if constexpr (std::is_same_v<T, Object>) {
      throw Node::Error("Node not an object.");
    } else if constexpr (std::is_same_v<T, Boolean>) {
      throw Node::Error("Node not a boolean.");
    } else if constexpr (std::is_same_v<T, Null>) {
      throw Node::Error("Node not a null.");
    } else if constexpr (std::is_same_v<T, Hole>) {
      throw Node::Error("Node not a hole.");
    }
  }
}
template<typename T> T &NRef(Node &jNode)
{
  checkNode<T>(jNode);
  return static_cast<T &>(jNode.getVariant());
}
template<typename T> const T &NRef(const Node &jNode)
{
  checkNode<T>(jNode);
  return static_cast<const T &>(jNode.getVariant());
}
template<typename T> T &NRef(Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return static_cast<T &>(jNodeEntry.getNode().getVariant());
}
template<typename T> const T &NRef(const Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return static_cast<const T &>(jNodeEntry.getNode().getVariant());
}
}// namespace JSON_Lib