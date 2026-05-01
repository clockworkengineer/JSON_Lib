#pragma once

#include <string>
#include <string_view>
#include <type_traits>

namespace JSON_Lib {

// =======================
// What is Node variant ?
// =======================
template<typename T> bool isA(const Node &jNode) { return jNode.template is<T>(); }

// ================================
// Node type name for error messages
// ================================
template<typename T> constexpr std::string_view nodeTypeName()
{
  using U = std::remove_cv_t<T>;
  if constexpr (std::is_same_v<U, String>) { return "a string"; }
  else if constexpr (std::is_same_v<U, Number>) { return "a number"; }
  else if constexpr (std::is_same_v<U, Array>) { return "an array"; }
  else if constexpr (std::is_same_v<U, Object>) { return "an object"; }
  else if constexpr (std::is_same_v<U, Boolean>) { return "a boolean"; }
  else if constexpr (std::is_same_v<U, Null>) { return "a null"; }
  else if constexpr (std::is_same_v<U, Hole>) { return "a hole"; }
  else { return "an unknown type"; }
}

// =========================
// Node reference converter
// =========================
template<typename T> void checkNode(const Node &jNode)
{
  if (!isA<T>(jNode)) {
    throw Node::Error(std::string("Node not ").append(nodeTypeName<T>()).append("."));
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