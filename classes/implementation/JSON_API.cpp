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
                m_jNodeRoot = std::make_unique<JNodeObject>();
            }
            return ((*m_jNodeRoot)[key]);
        }
        catch ([[maybe_unused]] JNode::Error &error)
        {
            JNodeRef<JNodeObject>(*m_jNodeRoot).objects().emplace_back(JNodeObjectEntry{key, std::make_unique<JNode>()});
            return (*JNodeRef<JNodeObject>(*m_jNodeRoot).objects().back().second);
        }
    }
    const JNode &JSON::operator[](const std::string &key) const // Object
    {
        return ((*m_jNodeRoot)[key]);
    }
} // namespace JSONLib
