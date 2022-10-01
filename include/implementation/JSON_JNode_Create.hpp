#pragma once
// =======
// C++ STL
// =======
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
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
  if (auto pValue = std::get_if<std::nullptr_t>(&type)) { return (JNode(nullptr)); }
  if (auto pValue = std::get_if<JNode>(&type)) { return (std::move(*const_cast<JNode *>(pValue))); }
  throw Error("JNode of non-existant type could not be created.");
}
// ==================
// JNode constructors
// ==================
// Construct JNode from raw values
inline JNode::JNode(int value) { *this = Number::make(value); }
inline JNode::JNode(long value) { *this = Number::make(value); }
inline JNode::JNode(long long value) { *this = Number::make(value); }
inline JNode::JNode(float value) { *this = Number::make(value); }
inline JNode::JNode(double value) { *this = Number::make(value); }
inline JNode::JNode(long double value) { *this = Number::make(value); }
inline JNode::JNode(const char *value) { *this = String::make(value); }
inline JNode::JNode(const std::string &value) { *this = String::make(value); }
inline JNode::JNode(bool value) { *this = Boolean::make(value); }
inline JNode::JNode([[maybe_unused]] std::nullptr_t value) { *this = Null::make(); }
// Construct JNode array from initializer list
inline JNode::JNode(const std::initializer_list<JSON::InternalType> &array)
{
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) { jNodeArrayList.emplace_back(internalTypeToJNode(entry)); }
  *this = Array::make(jNodeArrayList);
}
// Construct JNode object from initializer list
inline JNode::JNode(const std::initializer_list<std::pair<std::string, JSON::InternalType>> &object)
{
  Object::EntryList jObjectList;
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
