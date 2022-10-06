#pragma once
// =======
// C++ STL
// =======
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ==================
// JNode constructors
// ==================
// Construct JNode from raw values
template<typename T> JNode::JNode(T value)
{
  if constexpr (std::is_same_v<T, bool>) {
    *this = Boolean::make(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    *this = Number::make(value);
  } else if constexpr (std::is_same_v<T, nullptr_t>) {
    *this = Null::make();
  } else if constexpr (std::is_same_v<T, const char *>) {
    *this = String::make(value);
  } else if constexpr (std::is_same_v<T, std::string>) {
    *this = String::make(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    m_variant = std::move(value);
  }
}
// Construct JNode Array from initializer list
inline JNode::JNode(const JSON::ArrayInitializerList &array)
{
  std::vector<JNode> jNodeArray;
  for (const auto &entry : array) { jNodeArray.emplace_back(internalTypeToJNode(entry)); }
  *this = Array::make(jNodeArray);
}
// Construct JNode Object from initializer list
inline JNode::JNode(const JSON::ObjectInitializerList &object)
{
  std::vector<Object::Entry> jObjectList;
  for (const auto &entry : object) {
    jObjectList.emplace_back(Object::Entry(entry.first, internalTypeToJNode(entry.second)));
  }
  *this = Object::make(jObjectList);
}
// =====================
// JNode index overloads
// =====================
// Object
inline JNode &JNode::operator[](const std::string &key)
{
  if (this->isHole()) {
    *this = Object::make();
    JRef<Object>(*this).getObjectEntries().emplace_back(Object::Entry(key, Hole::make()));
    return (JRef<Object>(*this).getObjectEntries().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
inline const JNode &JNode::operator[](const std::string &key) const { return (JRef<const Object>(*this)[key]); }
// Array
inline JNode &JNode::operator[](std::size_t index)
{
  try {
    if (this->isHole()) { *this = Array::make(); }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).resize(index);
    return (JRef<Array>(*this)[index]);
  }
}
inline const JNode &JNode::operator[](std::size_t index) const { return (JRef<Array>(*this)[index]); }
}// namespace JSON_Lib
