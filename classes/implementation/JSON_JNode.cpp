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
// Create JNode for passed in type
JNode JNode::getJNode(const JSON::InternalTypes &type)
{
  JNode jNode;
  if (auto pInteger = std::get_if<int>(&type)) {
    jNode = Number::make(Numeric{ *pInteger });
  } else if (auto pLong = std::get_if<long>(&type)) {
    jNode = Number::make(Numeric{ *pLong });
  } else if (auto pFLoat = std::get_if<float>(&type)) {
    jNode = Number::make(Numeric{ *pFLoat });
  } else if (auto pDouble = std::get_if<double>(&type)) {
    jNode = Number::make(Numeric{ *pDouble });
  } else if (auto pString = std::get_if<std::string>(&type)) {
    jNode = String::make(*pString);
  } else if (auto pBoolean = std::get_if<bool>(&type)) {
    jNode = Boolean::make(*pBoolean);
  } else if (auto pNull = std::get_if<std::nullptr_t>(&type)) {
    jNode = Null::make();
  } else if (auto pJNode = std::get_if<JNode>(&type)) {
    jNode = std::move(*const_cast<JNode *>(pJNode));
  }
  return (jNode);
}
// ==============
// PUBLIC METHODS
// ==============
// ==================
// JNode constructors
// ==================
// Construct JNode from raw values
JNode::JNode(int integer) { *this = Number::make(Numeric{ integer }); }
JNode::JNode(long integer) { *this = Number::make(Numeric{ integer }); }
JNode::JNode(float floatingPoint) { *this = Number::make(Numeric{ floatingPoint }); }
JNode::JNode(double floatingPoint) { *this = Number::make(Numeric{ floatingPoint }); }
JNode::JNode(const char *cString) { *this = String::make(cString); }
JNode::JNode(const std::string &string) { *this = String::make(string); }
JNode::JNode(bool boolean) { *this = Boolean::make(boolean); }
JNode::JNode([[maybe_unused]] std::nullptr_t null) { *this = Null::make(); }
// Construct JNode array from initializer list
JNode::JNode(const std::initializer_list<JSON::InternalTypes> &array)
{
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) { jNodeArrayList.emplace_back(getJNode(entry)); }
  *this = Array::make(jNodeArrayList);
}
// Construct JNode object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, JSON::InternalTypes>> &object)
{
  Object::EntryList jObjectList;
  for (const auto &entry : object) { jObjectList.emplace_back(Object::Entry(entry.first, getJNode(entry.second))); }
  *this = Object::make(jObjectList);
}
// =========================
// Interrogate Jnode variant
// =========================
bool JNode::isObject() const { return (m_jNodeVariant->getType() == JNodeType::object); }
bool JNode::isArray() const { return (m_jNodeVariant->getType() == JNodeType::array); }
bool JNode::isNumber() const { return (m_jNodeVariant->getType() == JNodeType::number); }
bool JNode::isString() const { return (m_jNodeVariant->getType() == JNodeType::string); }
bool JNode::isBoolean() const { return (m_jNodeVariant->getType() == JNodeType::boolean); }
bool JNode::isNull() const { return (m_jNodeVariant->getType() == JNodeType::null); }
bool JNode::isHole() const { return (m_jNodeVariant->getType() == JNodeType::hole); }
// =====================
// JNode index overloads
// =====================
// Object
JNode &JNode::operator[](const std::string &key) 
{
  if (this->getType() == JNodeType::hole) {
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
    if (this->getType() == JNodeType::hole) { *this = Array::make(); }
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
JNode &JNode::operator=(int integer)
{
  *this = Number::make(Numeric{ integer });
  return (*this);
}
JNode &JNode::operator=(long integer)
{
  *this = Number::make(Numeric{ integer });
  return (*this);
}
JNode &JNode::operator=(float floatingPoint)
{
  *this = Number::make(Numeric{ floatingPoint });
  return (*this);
}
JNode &JNode::operator=(double floatingPoint)
{
  *this = Number::make(Numeric{ floatingPoint });
  return (*this);
}
JNode &JNode::operator=(const char *cString)
{
  *this = String::make(cString);
  return (*this);
}
JNode &JNode::operator=(const std::string &string)
{
  *this = String::make(string);
  return (*this);
}
JNode &JNode::operator=(bool boolean)
{
  *this = Boolean::make(boolean);
  return (*this);
}
JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null)
{
  *this = Null::make();
  return (*this);
}
// ==============
// Get JNode type
// ==============
JNodeType JNode::getType() const { return (m_jNodeVariant->getType()); }
// ==============================
// Get reference to JNode variant
// ==============================
std::unique_ptr<Variant> &JNode::getVariant() { return (m_jNodeVariant); }
const std::unique_ptr<Variant> &JNode::getVariant() const { return (m_jNodeVariant); }
}// namespace JSONLib
