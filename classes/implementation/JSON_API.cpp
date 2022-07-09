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
    //
    // Object
    //
    JNode &JSON::operator[](const std::string &key)
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
            JNodeDataRef<JNodeObjectData>(*m_jNodeRoot).objects().emplace_back(JNodeObjectEntry{key, makeJNodeHole()});
            return (*JNodeDataRef<JNodeObjectData>(*m_jNodeRoot).objects().back().value);
        }
    }
    const JNode &JSON::operator[](const std::string &key) const // Object
    {
        return ((*m_jNodeRoot)[key]);
    }
    //
    // Array
    //
    JNode &JSON::operator[](std::size_t index)
    {
        try
        {
            if (m_jNodeRoot == nullptr)
            {
                m_jNodeRoot = m_jsonImplementation->parse("[]");
            }
            return (*JNodeDataRef<JNodeArrayData>(*m_jNodeRoot).array().at(index));
        }
        catch ([[maybe_unused]] std::out_of_range const &error)
        {
            JNodeDataRef<JNodeArrayData>(*m_jNodeRoot).array().resize(index+1);
            JNodeDataRef<JNodeArrayData>(*m_jNodeRoot).array()[index] = makeJNodeNull();
            return (*JNodeDataRef<JNodeArrayData>(*m_jNodeRoot).array().at(index));
        }
    }
    const JNode &JSON::operator[](std::size_t index) const // Object
    {
        return ((*m_jNodeRoot)[index]);
    }
} // namespace JSONLib
