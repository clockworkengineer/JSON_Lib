//
// Class: JSON
//
// Description: Class to perform JSON  stringify/parse to/from a byte
// or file. It is also  possible to customize this with the ISource
// and IDestination interfaces if required. Note: At present it will
// report incorrect JSON syntax but will not be specific about what
// error has occurred; this is reasoned to add too much overhead to
// the process of parsing for the requirements of this library
// (this might change in future versions). For an in-depth description
// of the JSON specification refer to its RFC at web address
// https://tools.ietf.org/html/rfc8259.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSONConfig.hpp"
#include "JSON.hpp"
#include "JSONImplementation.hpp"
#include "JSONTranslator.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
//
// C++ STL
//
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
    JSON::JSON(ITranslator *translator)
    {
        m_imp = new (JSONImplementation);
        m_imp->translator(translator);
    }
    JSON::~JSON()
    {
        delete m_imp;
    }
    JNode &JSON::root()
    {
        return (m_imp->root());
    }
    /// <summary>
    /// Strip all whitespace from a JSON source .
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destinaton">Destination for stripped JSON</param>
    void JSON::strip(ISource &source, IDestination &destination)
    {
        m_imp->strip(source, destination);
    }
    void JSON::strip(ISource &source, IDestination &&destination)
    {
        m_imp->strip(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &destination)
    {
        m_imp->strip(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &&destination)
    {
        m_imp->strip(source, destination);
    }
    /// <summary>
    /// Create JNode structure by recursively parsing JSON on the source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.
    void JSON::parse(ISource &source)
    {
        m_imp->parse(source);
    }
    void JSON::parse(ISource &&source)
    {
        m_imp->parse(source);
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON in destination stream.
    /// </summary>
    /// <param name="jNodeRoot">Root of JNode structure.</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON::stringify(IDestination &destination)
    {
        m_imp->stringify(destination);
    }
    void JSON::stringify(IDestination &&destination)
    {
        m_imp->stringify(destination);
    }
} // namespace JSONLib
