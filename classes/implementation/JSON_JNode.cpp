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
// ==============
// PUBLIC METHODS
// ==============
// ==================
// JNode constructors
// ==================
// Construct JSON array from initializer list
JNode::JNode(const std::initializer_list<InternalTypes> &array) {
  Array::ArrayList jNodeArrayList;
  for (const auto &entry : array) {
    if (const int *pint = std::get_if<int>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*pint}));
    } else if (const long *plong = std::get_if<long>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*plong}));
    } else if (const float *pfloat = std::get_if<float>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*pfloat}));
    } else if (const double *pdouble = std::get_if<double>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*pdouble}));
    } else if (const std::string *pstring = std::get_if<std::string>(&entry)) {
      jNodeArrayList.emplace_back(makeString(*pstring));
    } else if (const bool *pboolean = std::get_if<bool>(&entry)) {
      jNodeArrayList.emplace_back(makeBoolean(*pboolean));
    } else if (const std::nullptr_t *pnull =
                   std::get_if<std::nullptr_t>(&entry)) {
      jNodeArrayList.emplace_back(makeNull());
    }
  }
  JNode jNode{makeArray(jNodeArrayList)};
  std::swap(*this, jNode);
}
// Construct JSON object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, InternalTypes>>
                 &object) {
  Object::EntryList jObjectList;
  for (const auto &entry : object) {
    JNode jNode;
    if (const int *pint = std::get_if<int>(&entry.second)) {
      jNode = makeNumber(Numeric{*pint});
    } else if (const long *plong = std::get_if<long>(&entry.second)) {
      jNode = makeNumber(Numeric{*plong});
    } else if (const float *pfloat = std::get_if<float>(&entry.second)) {
      jNode = makeNumber(Numeric{*pfloat});
    } else if (const double *pdouble = std::get_if<double>(&entry.second)) {
      jNode = makeNumber(Numeric{*pdouble});
    } else if (const std::string *pstring =
                   std::get_if<std::string>(&entry.second)) {
      jNode = makeString(*pstring);
    } else if (const bool *pboolean = std::get_if<bool>(&entry.second)) {
      jNode = makeBoolean(*pboolean);
    } else if (const std::nullptr_t *pnull =
                   std::get_if<std::nullptr_t>(&entry.second)) {
      jNode = makeNull();
    }
    jObjectList.emplace_back(Object::Entry(entry.first, jNode));
  }
  JNode jNode{makeObject(jObjectList)};
  std::swap(*this, jNode);
}
// =====================
// JNode index overloads
// =====================
// ======
// Object
// ======
JNode &JNode::operator[](const std::string &key) {
  if (this->getType() == JNodeType::hole) {
    this->m_jNodeVariant = std::make_unique<Object>();
    JRef<Object>(*this).objectEntries().emplace_back(
        Object::Entry(key, makeHole()));
    return (JRef<Object>(*this).objectEntries().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
const JNode &JNode::operator[](const std::string &key) const {
  return (JRef<const Object>(*this)[key]);
}
// =====
// Array
// =====
JNode &JNode::operator[](std::size_t index) {
  try {
    if (this->getType() == JNodeType::hole) {
      this->m_jNodeVariant = std::make_unique<Array>();
    }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).array().resize(index + 1);
    JRef<Array>(*this).array()[index] = makeHole();
    for (auto &entry : JRef<Array>(*this).array()) {
      if (entry.getVariant() == nullptr) {
        entry = makeHole();
      }
    }
    return (JRef<Array>(*this)[index]);
  }
}
const JNode &JNode::operator[](std::size_t index) const {
  return (JRef<Array>(*this)[index]);
}
// ==========================
// JNode assignment operators
// ==========================
JNode &JNode::operator=(float floatingPoint) {
  JNode jNode{makeNumber(Numeric{floatingPoint})};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(double floatingPoint) {
  JNode jNode{makeNumber(Numeric{floatingPoint})};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(int integer) {
  JNode jNode{makeNumber(Numeric{integer})};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(long integer) {
  JNode jNode{makeNumber(Numeric{integer})};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(const char *cString) {
  JNode jNode{makeString(cString)};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(const std::string &string) {
  JNode jNode{makeString(string)};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=(bool boolean) {
  JNode jNode{makeBoolean(boolean)};
  std::swap(*this, jNode);
  return (*this);
}
JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null) {
  JNode jNode{makeNull()};
  std::swap(*this, jNode);
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
const std::unique_ptr<Variant> &JNode::getVariant() const {
  return (m_jNodeVariant);
}
} // namespace JSONLib
