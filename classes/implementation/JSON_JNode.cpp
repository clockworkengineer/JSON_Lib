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
    } else if (const long long *plonglong = std::get_if<long long>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*plonglong}));
    } else if (const float *pfloat = std::get_if<float>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*pfloat}));
    } else if (const double *pdouble = std::get_if<double>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*pdouble}));
    } else if (const long double *plongdouble =
                   std::get_if<long double>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(Numeric{*plongdouble}));
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
    Object::Entry jNodeObjectEntry;
    jNodeObjectEntry.key = entry.first;
    if (const int *pint = std::get_if<int>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*pint});
    } else if (const long *plong = std::get_if<long>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*plong});
    } else if (const long long *plonglong =
                   std::get_if<long long>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*plonglong});
    } else if (const float *pfloat = std::get_if<float>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*pfloat});
    } else if (const double *pdouble = std::get_if<double>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*pdouble});
    } else if (const long double *plongdouble =
                   std::get_if<long double>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(Numeric{*plongdouble});
    } else if (const std::string *pstring =
                   std::get_if<std::string>(&entry.second)) {
      jNodeObjectEntry.value = makeString(*pstring);
    } else if (const bool *pboolean = std::get_if<bool>(&entry.second)) {
      jNodeObjectEntry.value = makeBoolean(*pboolean);
    } else if (const std::nullptr_t *pnull =
                   std::get_if<std::nullptr_t>(&entry.second)) {
      jNodeObjectEntry.value = makeNull();
    }
    jObjectList.emplace_back(std::move(jNodeObjectEntry));
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
    JNodeRef<Object>(*this).objects().emplace_back(
        Object::Entry{key, makeHole()});
    return (JNodeRef<Object>(*this).objects().back().value);
  }
  return (JNodeRef<Object>(*this)[key]);
}
const JNode &JNode::operator[](const std::string &key) const {
  return (JNodeRef<const Object>(*this)[key]);
}
// =====
// Array
// =====
JNode &JNode::operator[](std::size_t index) {
  try {
    if (this->getType() == JNodeType::hole) {
      this->m_jNodeVariant = std::make_unique<Array>();
    }
    return (JNodeRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JNodeRef<Array>(*this).array().resize(index + 1);
    JNodeRef<Array>(*this).array()[index] = makeHole();
    for (auto &entry : JNodeRef<Array>(*this).array()) {
      if (entry.getVariant() == nullptr) {
        entry = makeHole();
      }
    }
    return (JNodeRef<Array>(*this)[index]);
  }
}
const JNode &JNode::operator[](std::size_t index) const {
  return (JNodeRef<Array>(*this)[index]);
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
JNode &JNode::operator=(long double floatingPoint) {
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
JNode &JNode::operator=(long long integer) {
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
