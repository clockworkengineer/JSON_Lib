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
    jNode = makeNumber(Numeric{ *pInteger });
  } else if (auto pLong = std::get_if<long>(&type)) {
    jNode = makeNumber(Numeric{ *pLong });
  } else if (auto pFLoat = std::get_if<float>(&type)) {
    jNode = makeNumber(Numeric{ *pFLoat });
  } else if (auto pDouble = std::get_if<double>(&type)) {
    jNode = makeNumber(Numeric{ *pDouble });
  } else if (auto pString = std::get_if<std::string>(&type)) {
    jNode = makeString(*pString);
  } else if (auto pBoolean = std::get_if<bool>(&type)) {
    jNode = makeBoolean(*pBoolean);
  } else if (auto pNull = std::get_if<std::nullptr_t>(&type)) {
    jNode = makeNull();
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
JNode::JNode(int integer) { *this = makeNumber(Numeric{ integer }); }
JNode::JNode(long integer) { *this = makeNumber(Numeric{ integer }); }
JNode::JNode(float floatingPoint) { *this = makeNumber(Numeric{ floatingPoint }); }
JNode::JNode(double floatingPoint) { *this = makeNumber(Numeric{ floatingPoint }); }
JNode::JNode(const char *cString) { *this = makeString(cString); }
JNode::JNode(const std::string &string) { *this = makeString(string); }
JNode::JNode(bool boolean) { *this = makeBoolean(boolean); }
JNode::JNode([[maybe_unused]] std::nullptr_t null) { *this = makeNull(); }
// Construct JNode array from initializer list
JNode::JNode(const std::initializer_list<JSON::InternalTypes> &array)
{
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) { jNodeArrayList.emplace_back(getJNode(entry)); }
  *this = makeArray(jNodeArrayList);
}
// Construct JNode object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, JSON::InternalTypes>> &object)
{
  Object::EntryList jObjectList;
  for (const auto &entry : object) { jObjectList.emplace_back(Object::Entry(entry.first, getJNode(entry.second))); }
  *this = makeObject(jObjectList);
}
// =====================
// JNode index overloads
// =====================
// ======
// Object
// ======
JNode &JNode::operator[](const std::string &key)
{
  if (this->getType() == JNodeType::hole) {
    *this = makeObject();
    JRef<Object>(*this).getObjectEntries().emplace_back(Object::Entry(key, makeHole()));
    return (JRef<Object>(*this).getObjectEntries().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
const JNode &JNode::operator[](const std::string &key) const { return (JRef<const Object>(*this)[key]); }
// =====
// Array
// =====
JNode &JNode::operator[](std::size_t index)
{
  try {
    if (this->getType() == JNodeType::hole) { *this = makeArray(); }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).getArrayEntries().resize(index + 1);
    JRef<Array>(*this).getArrayEntries()[index] = makeHole();
    for (auto &entry : JRef<Array>(*this).getArrayEntries()) {
      if (entry.getVariant() == nullptr) { entry = makeHole(); }
    }
    return (JRef<Array>(*this)[index]);
  }
}
const JNode &JNode::operator[](std::size_t index) const { return (JRef<Array>(*this)[index]); }
// ==========================
// JNode assignment operators
// ==========================
JNode &JNode::operator=(int integer)
{
  *this = makeNumber(Numeric{ integer });
  return (*this);
}
JNode &JNode::operator=(long integer)
{
  *this = makeNumber(Numeric{ integer });
  return (*this);
}
JNode &JNode::operator=(float floatingPoint)
{
  *this = makeNumber(Numeric{ floatingPoint });
  return (*this);
}
JNode &JNode::operator=(double floatingPoint)
{
  *this = makeNumber(Numeric{ floatingPoint });
  return (*this);
}
JNode &JNode::operator=(const char *cString)
{
  *this = makeString(cString);
  return (*this);
}
JNode &JNode::operator=(const std::string &string)
{
  *this = makeString(string);
  return (*this);
}
JNode &JNode::operator=(bool boolean)
{
  *this = makeBoolean(boolean);
  return (*this);
}
JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null)
{
  *this = makeNull();
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
