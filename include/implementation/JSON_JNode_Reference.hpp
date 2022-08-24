#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <unordered_map>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =========================
// JNode reference converter
// =========================
template<typename T> void CheckJNodeType(const Variant &jNodeVariant)
{
  if constexpr (std::is_same_v<T, String>) {
    if (jNodeVariant.getType() != JNodeType::string) { throw JNode::Error("Node not a string."); }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (jNodeVariant.getType() != JNodeType::number) { throw JNode::Error("Node not a number."); }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (jNodeVariant.getType() != JNodeType::array) { throw JNode::Error("Node not an array."); }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (jNodeVariant.getType() != JNodeType::object) { throw JNode::Error("Node not an object."); }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (jNodeVariant.getType() != JNodeType::boolean) { throw JNode::Error("Node not an boolean."); }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (jNodeVariant.getType() != JNodeType::null) { throw JNode::Error("Node not a null."); }
  }
}
template<typename T> T &JRef(JNode &jNode)
{
  CheckJNodeType<T>(*jNode.getVariant());
  return (static_cast<T &>(*jNode.getVariant()));
}
template<typename T> const T &JRef(const JNode &jNode)
{
  CheckJNodeType<T>(*jNode.getVariant());
  return (static_cast<const T &>(*jNode.getVariant()));
}
template<typename T> T &JRef(Object::Entry &jNodeEntry)
{
  CheckJNodeType<T>(*jNodeEntry.getJNode().getVariant());
  return (static_cast<T &>(*jNodeEntry.getJNode().getVariant()));
}
template<typename T> const T &JRef(const Object::Entry &jNodeEntry)
{
  CheckJNodeType<T>(*jNodeEntry.getJNode().getVariant());
  return (static_cast<const T &>(*jNodeEntry.getJNode().getVariant()));
}
}// namespace JSONLib