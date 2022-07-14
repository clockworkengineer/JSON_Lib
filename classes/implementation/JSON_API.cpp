//
// Class: JSON_API
//
// Description: JSON creation api functionality.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Impl.hpp"
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
// ======
// Object
// ======
JNode &JSON::operator[](const std::string &key) {
  try {
    if (m_jNodeRoot == nullptr) {
      m_jNodeRoot = m_jsonImplementation->parse("{}");
    }
    return ((*m_jNodeRoot)[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JNodeRef<JNodeObject>(*m_jNodeRoot)
        .objects()
        .emplace_back(JNodeObjectEntry{key, makeHole()});
    return (*JNodeRef<JNodeObject>(*m_jNodeRoot).objects().back().value);
  }
}
const JNode &JSON::operator[](const std::string &key) const // Object
{
  return ((*m_jNodeRoot)[key]);
}
// =====
// Array
// =====
JNode &JSON::operator[](std::size_t index) {
  try {
    if (m_jNodeRoot == nullptr) {
      m_jNodeRoot = m_jsonImplementation->parse("[]");
    }
    return ((*m_jNodeRoot)[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JNodeRef<JNodeArray>(*m_jNodeRoot).array().resize(index + 1);
    JNodeRef<JNodeArray>(*m_jNodeRoot).array()[index] =
        std::move(makeNull());
    return (*JNodeRef<JNodeArray>(*m_jNodeRoot).array()[index]);
  }
}
const JNode &JSON::operator[](std::size_t index) const {
  return ((*m_jNodeRoot)[index]);
}
} // namespace JSONLib
