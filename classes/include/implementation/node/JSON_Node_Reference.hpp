#pragma once

#include <type_traits>

namespace JSON_Lib {

// =======================
// What is Node variant ?
// =======================
template<typename T> bool isA(const Node &jNode)
{
  if constexpr (std::is_same_v<T, String>) {
    return jNode.template is<String>();
  } else if constexpr (std::is_same_v<T, Number>) {
    return jNode.template is<Number>();
  } else if constexpr (std::is_same_v<T, Array>) {
    return jNode.template is<Array>();
  } else if constexpr (std::is_same_v<T, Object>) {
    return jNode.template is<Object>();
  } else if constexpr (std::is_same_v<T, Boolean>) {
    return jNode.template is<Boolean>();
  } else if constexpr (std::is_same_v<T, Null>) {
    return jNode.template is<Null>();
  } else if constexpr (std::is_same_v<T, Hole>) {
    return jNode.template is<Hole>();
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
  return jNode.template get<T>();
}
template<typename T> const T &NRef(const Node &jNode)
{
  checkNode<T>(jNode);
  return jNode.template get<T>();
}
template<typename T> T &NRef(Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return jNodeEntry.getNode().template get<T>();
}
template<typename T> const T &NRef(const Object::Entry &jNodeEntry)
{
  checkNode<T>(jNodeEntry.getNode());
  return jNodeEntry.getNode().template get<T>();
}
}// namespace JSON_Lib