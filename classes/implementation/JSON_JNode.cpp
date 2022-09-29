//
// Class: JNode
//
// Description: JNode struct is used to hold the parsed JSON and model its
// structure by arranging the JNodes in a tree; this can then be traversed to
// reform the JSON text during stringification.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Types.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ===========================
// PRIVATE TYPES AND CONSTANTS
// ===========================
// ==========================
// PUBLIC TYPES AND CONSTANTS
// ==========================
// ========================
// PRIVATE STATIC VARIABLES
// ========================
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// =================
// PRIVATE FUNCTIONS
// =================
/// <summary>
/// Create JNode from passed in type.
/// </summary>
/// <param name="type">Variant encapsulating type and value to be turned into JNode.</param>
/// <returns>JNode for passed in type.</returns>
JNode internalTypeToJNode(const JSON::InternalType &type)
{
  if (auto pInteger = std::get_if<int>(&type)) {
    return (Number::make(*pInteger));
  } else if (auto pLong = std::get_if<long>(&type)) {
    return (Number::make(*pLong));
  } else if (auto pLongLong = std::get_if<long long>(&type)) {
    return (Number::make(*pLongLong));
  } else if (auto pFLoat = std::get_if<float>(&type)) {
    return (Number::make(*pFLoat));
  } else if (auto pDouble = std::get_if<double>(&type)) {
    return (Number::make(*pDouble));
  } else if (auto pLongDouble = std::get_if<long double>(&type)) {
    return (Number::make(*pLongDouble));
  } else if (auto pString = std::get_if<std::string>(&type)) {
    return (String::make(*pString));
  } else if (auto pBoolean = std::get_if<bool>(&type)) {
    return (Boolean::make(*pBoolean));
  } else if (auto pNull = std::get_if<std::nullptr_t>(&type)) {
    return (Null::make());
  } else if (auto pJNode = std::get_if<JNode>(&type)) {
    return (std::move(*const_cast<JNode *>(pJNode)));
  }
  throw Error("JNode of non-existant type could not be created.");
}
// ===============
// PRIVATE METHODS
// ===============
// ==============
// PUBLIC METHODS
// ==============
// ==================
// JNode constructors
// ==================
// Construct JNode from raw values
JNode::JNode(int value) { *this = Number::make(value); }
JNode::JNode(long value) { *this = Number::make(value); }
JNode::JNode(long long value) { *this = Number::make(value); }
JNode::JNode(float value) { *this = Number::make(value); }
JNode::JNode(double value) { *this = Number::make(value); }
JNode::JNode(long double value) { *this = Number::make(value); }
JNode::JNode(const char *value) { *this = String::make(value); }
JNode::JNode(const std::string &value) { *this = String::make(value); }
JNode::JNode(bool value) { *this = Boolean::make(value); }
JNode::JNode([[maybe_unused]] std::nullptr_t value) { *this = Null::make(); }
// Construct JNode array from initializer list
JNode::JNode(const std::initializer_list<JSON::InternalType> &array)
{
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) { jNodeArrayList.emplace_back(internalTypeToJNode(entry)); }
  *this = Array::make(jNodeArrayList);
}
// Construct JNode object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, JSON::InternalType>> &object)
{
  Object::EntryList jObjectList;
  for (const auto &entry : object) {
    jObjectList.emplace_back(Object::Entry(entry.first, internalTypeToJNode(entry.second)));
  }
  *this = Object::make(jObjectList);
}
// =========================
// Interrogate JNode variant
// =========================
bool JNode::isEmpty() const { return (m_variant == nullptr); }
bool JNode::isObject() const { return (m_variant->getType() == Variant::Type::object); }
bool JNode::isArray() const { return (m_variant->getType() == Variant::Type::array); }
bool JNode::isNumber() const { return (m_variant->getType() == Variant::Type::number); }
bool JNode::isString() const { return (m_variant->getType() == Variant::Type::string); }
bool JNode::isBoolean() const { return (m_variant->getType() == Variant::Type::boolean); }
bool JNode::isNull() const { return (m_variant->getType() == Variant::Type::null); }
bool JNode::isHole() const { return (m_variant->getType() == Variant::Type::hole); }
// =====================
// JNode index overloads
// =====================
// Object
JNode &JNode::operator[](const std::string &key)
{
  if (this->isHole()) {
    *this = Object::make();
    JRef<Object>(*this).getObjectEntries().emplace_back(Object::Entry(key, Hole::make()));
    return (JRef<Object>(*this).getObjectEntries().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
const JNode &JNode::operator[](const std::string &key) const { return (JRef<const Object>(*this)[key]); }
// Array
JNode &JNode::operator[](std::size_t index)
{
  try {
    if (this->isHole()) { *this = Array::make(); }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).resize(index);
    return (JRef<Array>(*this)[index]);
  }
}
const JNode &JNode::operator[](std::size_t index) const { return (JRef<Array>(*this)[index]); }
// ==============================
// Get reference to JNode variant
// ==============================
Variant &JNode::getVariant() { return (*m_variant); }
const Variant &JNode::getVariant() const { return (*m_variant); }
}// namespace JSON_Lib
