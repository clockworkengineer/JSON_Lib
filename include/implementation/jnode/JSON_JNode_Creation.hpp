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
  } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
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
  *this = JNode::make<Array>();
  for (const auto &entry : array) { JRef<Array>(*this).add(internalTypeToJNode(entry)); }
}
// Construct JNode Object from initializer list
inline JNode::JNode(const JSON::ObjectList &object)
{
  *this = JNode::make<Object>();
  for (const auto &entry : object) {
    JRef<Object>(*this).add(Object::Entry(entry.first, internalTypeToJNode(entry.second)));
  }
}
// =====================
// JNode index overloads
// =====================
// Object
inline JNode &JNode::operator[](const std::string &key)
{
  if (this->isHole()) {
    *this = JNode::make<Object>();
    JRef<Object>(*this).add(Object::Entry(key, JNode::make<Hole>()));
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
/// <summary>
/// Create JNode from passed in type.
/// </summary>
/// <param name="type">Variant encapsulating type and value to be turned into JNode.</param>
/// <returns>JNode for passed in type.</returns>
inline JNode JNode::internalTypeToJNode(const JSON::InternalType &type)
{
  if (auto pValue = std::get_if<int>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long long>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<float>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<double>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long double>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<std::string>(&type)) { return (JNode((*pValue))); }
  if (auto pValue = std::get_if<bool>(&type)) { return (JNode((*pValue))); }
  if ([[maybe_unused]] auto pValue = std::get_if<std::nullptr_t>(&type)) { return (JNode(nullptr)); }
  if (auto pValue = std::get_if<JNode>(&type)) { return (std::move(*const_cast<JNode *>(pValue))); }
  throw Error("JNode of non-existant type could not be created.");
}
}// namespace JSON_Lib
