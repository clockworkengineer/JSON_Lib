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
  JNodeArray::ArrayList jNodeArrayList;
  for (const auto &entry : array) {
    if (const int *pint = std::get_if<int>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*pint}));
    } else if (const long *plong = std::get_if<long>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*plong}));
    } else if (const long long *plonglong = std::get_if<long long>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*plonglong}));
    } else if (const float *pfloat = std::get_if<float>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*pfloat}));
    } else if (const double *pdouble = std::get_if<double>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*pdouble}));
    } else if (const long double *plongdouble =
                   std::get_if<long double>(&entry)) {
      jNodeArrayList.emplace_back(makeNumber(JNodeNumeric{*plongdouble}));
    } else if (const std::string *pstring = std::get_if<std::string>(&entry)) {
      jNodeArrayList.emplace_back(makeString(*pstring));
    } else if (const bool *pboolean = std::get_if<bool>(&entry)) {
      jNodeArrayList.emplace_back(makeBoolean(*pboolean));
    } else if (const std::nullptr_t *pnull =
                   std::get_if<std::nullptr_t>(&entry)) {
      jNodeArrayList.emplace_back(makeNull());
    }
  }
  std::swap(*this, *makeArray(jNodeArrayList));
}
// Construct JSON object from initializer list
JNode::JNode(const std::initializer_list<std::pair<std::string, InternalTypes>>
                 &object) {
  JNodeObject::EntryList jObjectList;
  for (const auto &entry : object) {
    JNodeObject::Entry jNodeObjectEntry;
    jNodeObjectEntry.key = entry.first;
    if (const int *pint = std::get_if<int>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*pint});
    } else if (const long *plong = std::get_if<long>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*plong});
    } else if (const long long *plonglong =
                   std::get_if<long long>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*plonglong});
    } else if (const float *pfloat = std::get_if<float>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*pfloat});
    } else if (const double *pdouble = std::get_if<double>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*pdouble});
    } else if (const long double *plongdouble =
                   std::get_if<long double>(&entry.second)) {
      jNodeObjectEntry.value = makeNumber(JNodeNumeric{*plongdouble});
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
  std::swap(*this, *makeObject(jObjectList));
}
// =====================
// JNode index overloads
// =====================
// ======
// Object
// ======
JNode &JNode::operator[](const std::string &key) {
  if (this->getNodeType() == JNodeType::hole) {
    this->m_jNodeVariant = std::make_unique<JNodeObject>();
    JNodeRef<JNodeObject>(*this).objects().emplace_back(
        JNodeObject::Entry{key, makeHole()});
    return (*JNodeRef<JNodeObject>(*this).objects().back().value);
  }
  return (JNodeRef<JNodeObject>(*this)[key]);
}
const JNode &JNode::operator[](const std::string &key) const {
  return (JNodeRef<const JNodeObject>(*this)[key]);
}
// =====
// Array
// =====
JNode &JNode::operator[](std::size_t index) {
  try {
    if (this->getNodeType() == JNodeType::hole) {
      this->m_jNodeVariant = std::make_unique<JNodeArray>();
    }
    return (JNodeRef<JNodeArray>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JNodeRef<JNodeArray>(*this).array().resize(index + 1);
    JNodeRef<JNodeArray>(*this).array()[index] = makeHole();
    for (auto &entry : JNodeRef<JNodeArray>(*this).array()) {
      if (entry == nullptr) {
        entry = makeHole();
      }
    }
    return (JNodeRef<JNodeArray>(*this)[index]);
  }
}
const JNode &JNode::operator[](std::size_t index) const {
  return (JNodeRef<JNodeArray>(*this)[index]);
}
// ==========================
// JNode assignment operators
// ==========================
JNode &JNode::operator=(float floatingPoint) {
  std::swap(*this, *makeNumber(JNodeNumeric{floatingPoint}));
  return (*this);
}
JNode &JNode::operator=(double floatingPoint) {
  std::swap(*this, *makeNumber(JNodeNumeric{floatingPoint}));
  return (*this);
}
JNode &JNode::operator=(long double floatingPoint) {
  std::swap(*this, *makeNumber(JNodeNumeric{floatingPoint}));
  return (*this);
}
JNode &JNode::operator=(int integer) {
  std::swap(*this, *makeNumber(JNodeNumeric{integer}));
  return (*this);
}
JNode &JNode::operator=(long integer) {
  std::swap(*this, *makeNumber(JNodeNumeric{integer}));
  return (*this);
}
JNode &JNode::operator=(long long integer) {
  std::swap(*this, *makeNumber(JNodeNumeric{integer}));
  return (*this);
}
JNode &JNode::operator=(const char *cString) {
  std::swap(*this, *makeString(cString));
  return (*this);
}
JNode &JNode::operator=(const std::string &string) {
  std::swap(*this, *makeString(string));
  return (*this);
}
JNode &JNode::operator=(bool boolean) {
  std::swap(*this, *makeBoolean(boolean));
  return (*this);
}
JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null) {
  std::swap(*this, *makeNull());
  return (*this);
}
// ==============
// Get JNode type
// ==============
JNodeType JNode::getNodeType() const { return (m_jNodeVariant->getNodeType()); }
// =============================
// Get reference to JNodeVariant
// =============================
std::unique_ptr<JNodeVariant> &JNode::getJNodeVariant() {
  return (m_jNodeVariant);
}
const std::unique_ptr<JNodeVariant> &JNode::getJNodeVariant() const {
  return (m_jNodeVariant);
}
} // namespace JSONLib
