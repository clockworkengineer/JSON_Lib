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
// =========
// NAMESPACE
// =========
namespace JSONLib {
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
// ===============
// PRIVATE METHODS
// ===============
// Create JNode from passed in type
JNode JNode::createJNodeFromType(const JSON::Types &type)
{
  if (auto pInteger = std::get_if<int>(&type)) {
    return (Number::make(Number{ *pInteger }));
  } else if (auto pLong = std::get_if<long>(&type)) {
    return (Number::make(Number{ *pLong }));
  } else if (auto pLongLong = std::get_if<long long>(&type)) {
    return (Number::make(Number{ *pLongLong }));
  } else if (auto pFLoat = std::get_if<float>(&type)) {
    return (Number::make(Number{ *pFLoat }));
  } else if (auto pDouble = std::get_if<double>(&type)) {
    return (Number::make(Number{ *pDouble }));
  } else if (auto pLongDouble = std::get_if<long double>(&type)) {
    return (Number::make(Number{ *pLongDouble }));
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
// ==============
// PUBLIC METHODS
// ==============
// ==================
// JNode constructors
// ==================
// Construct JNode from raw values
JNode::JNode(int integer) { *this = Number::make(Number{ integer }); }
JNode::JNode(long integer) { *this = Number::make(Number{ integer }); }
JNode::JNode(long long integer) { *this = Number::make(Number{ integer }); }
JNode::JNode(float floatingPoint) { *this = Number::make(Number{ floatingPoint }); }
JNode::JNode(double floatingPoint) { *this = Number::make(Number{ floatingPoint }); }
JNode::JNode(long double floatingPoint) { *this = Number::make(Number{ floatingPoint }); }
JNode::JNode(const char *cString) { *this = String::make(cString); }
JNode::JNode(const std::string &string) { *this = String::make(string); }
JNode::JNode(bool boolean) { *this = Boolean::make(boolean); }
JNode::JNode([[maybe_unused]] std::nullptr_t null) { *this = Null::make(); }
// Construct JNode array from initializer list
JNode::JNode(const std::initializer_list<JSON::Types> &array)
{
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) { jNodeArrayList.emplace_back(createJNodeFromType(entry)); }
  *this = Array::make(jNodeArrayList);
}
// Construct JNode object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, JSON::Types>> &object)
{
  Object::EntryList jObjectList;
  for (const auto &entry : object) {
    jObjectList.emplace_back(Object::Entry(entry.first, createJNodeFromType(entry.second)));
  }
  *this = Object::make(jObjectList);
}
// =========================
// Interrogate JNode variant
// =========================
bool JNode::isEmpty() const { return (m_variant == nullptr); }
bool JNode::isObject() const { return (m_variant->getType() == JNode::Type::object); }
bool JNode::isArray() const { return (m_variant->getType() == JNode::Type::array); }
bool JNode::isNumber() const { return (m_variant->getType() == JNode::Type::number); }
bool JNode::isString() const { return (m_variant->getType() == JNode::Type::string); }
bool JNode::isBoolean() const { return (m_variant->getType() == JNode::Type::boolean); }
bool JNode::isNull() const { return (m_variant->getType() == JNode::Type::null); }
bool JNode::isHole() const { return (m_variant->getType() == JNode::Type::hole); }
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
// ==========================
// JNode assignment operators
// ==========================
JNode &JNode::operator=(int integer) { return (*this = Number::make(Number{ integer })); }
JNode &JNode::operator=(long integer) { return (*this = Number::make(Number{ integer })); }
JNode &JNode::operator=(long long integer) { return (*this = Number::make(Number{ integer })); }
JNode &JNode::operator=(float floatingPoint) { return (*this = Number::make(Number{ floatingPoint })); }
JNode &JNode::operator=(double floatingPoint) { return (*this = Number::make(Number{ floatingPoint })); }
JNode &JNode::operator=(long double floatingPoint) { return (*this = Number::make(Number{ floatingPoint })); }
JNode &JNode::operator=(const char *cString) { return (*this = String::make(cString)); }
JNode &JNode::operator=(const std::string &string) { return (*this = String::make(string)); }
JNode &JNode::operator=(bool boolean) { return (*this = Boolean::make(boolean)); }
JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null) { return (*this = Null::make()); }
// ==============================
// Get reference to JNode variant
// ==============================
Variant &JNode::getVariant() { return (*m_variant); }
const Variant &JNode::getVariant() const { return (*m_variant); }
}// namespace JSONLib
