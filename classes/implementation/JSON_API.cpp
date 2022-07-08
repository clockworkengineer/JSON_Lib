//
// Class: JSON_API
//
// Description:
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
namespace JSONLib
{
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
    JNode &JSON::operator[](const std::string &key) // Object
    {
        try
        {
            if (m_jNodeRoot == nullptr)
            {
                m_jNodeRoot = m_jsonImplementation->parse("{}");
            }
            return ((*m_jNodeRoot)[key]);
        }
        catch ([[maybe_unused]] JNode::Error &error)
        {
            JNodeDataRef<JNodeObjectData>(*m_jNodeRoot).objects().emplace_back(JNodeObjectEntry{key, makeJNodeNull()});
            return (*JNodeDataRef<JNodeObjectData>(*m_jNodeRoot).objects().back().value);
        }
    }
    const JNode &JSON::operator[](const std::string &key) const // Object
    {
        return ((*m_jNodeRoot)[key]);
    }
} // namespace JSONLib
