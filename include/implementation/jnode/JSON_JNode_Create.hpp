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
    *this = JNode::make<Boolean>(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    *this = JNode::make<Number>(value);
  } else if constexpr (std::is_same_v<T, nullptr_t>) {
    *this = JNode::make<Null>();
  } else if constexpr (std::is_same_v<T, const char *>) {
    *this = JNode::make<String>(value);
  } else if constexpr (std::is_same_v<T, std::string>) {
    *this = JNode::make<String>(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    m_variant = std::move(value);
  }
}
// Construct JNode Array from initializer list
inline JNode::JNode(const JSON::ArrayList &array)
{
  std::vector<JNode> jNodeArray;
  for (const auto &entry : array) { jNodeArray.emplace_back(internalTypeToJNode(entry)); }
  *this = JNode::make<Array>(jNodeArray);
}
// Construct JNode Object from initializer list
inline JNode::JNode(const JSON::ObjectList &object)
{
  std::vector<Object::Entry> jObjectList;
  for (const auto &entry : object) {
    jObjectList.emplace_back(Object::Entry(entry.first, internalTypeToJNode(entry.second)));
  }
  *this = JNode::make<Object>(jObjectList);
}
// =====================
// JNode index overloads
// =====================
// Object
inline JNode &JNode::operator[](const std::string &key)
{
  if (this->isHole()) {
    *this = JNode::make<Object>();
    JRef<Object>(*this).getObjectEntries().emplace_back(Object::Entry(key, JNode::make<Hole>()));
    return (JRef<Object>(*this).getObjectEntries().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
inline const JNode &JNode::operator[](const std::string &key) const { return (JRef<const Object>(*this)[key]); }
// Array
inline JNode &JNode::operator[](std::size_t index)
{
  try {
    if (this->isHole()) { *this = JNode::make<Array>(); }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).resize(index);
    return (JRef<Array>(*this)[index]);
  }
}
inline const JNode &JNode::operator[](std::size_t index) const { return (JRef<Array>(*this)[index]); }
}// namespace JSON_Lib
