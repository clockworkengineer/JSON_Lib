//
// Class: JSON
//
// Description: Perform JSON  stringify/parse to/from a byte or
// file. It is also  possible to customize this with the ISource
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
    /// <summary>
    /// JSON constructor.
    /// </summary>
    /// <param name="translator">Pointer to translator interface</param>
    /// <param name="converter">Pointer to converter interface</param>
    JSON::JSON(ITranslator *translator, IConverter *converter) : m_jsonImplementation(std::make_unique<JSON_Impl>())
    {
        m_jsonImplementation->converter(converter);
        m_jsonImplementation->translator(translator);
    }
    /// <summary>
    /// JSON constructor.
    /// </summary>
    /// <param name="jsonString">JSON string</param>
    JSON::JSON(const std::string &jsonString) : JSON()
    {
        m_jNodeRoot = m_jsonImplementation->parse(jsonString);
    }
    /// <summary>
    /// JSON destructor.
    /// </summary>
    JSON::~JSON()
    {
    }
    /// <summary>
    /// Get JSONLib version.
    /// </summary>
    std::string JSON::version()
    {
        return (m_jsonImplementation->version());
    }
    /// <summary>
    /// Strip all whitespace from a JSON source.
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destination">Destination for stripped JSON</param>
    void JSON::strip(ISource &source, IDestination &destination)
    {
        m_jsonImplementation->strip(source, destination);
    }
    void JSON::strip(ISource &source, IDestination &&destination)
    {
        m_jsonImplementation->strip(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &destination)
    {
        m_jsonImplementation->strip(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &&destination)
    {
        m_jsonImplementation->strip(source, destination);
    }
    /// <summary>
    /// Create JNode structure by recursively parsing JSON on the source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.
    void JSON::parse(ISource &source)
    {
        m_jNodeRoot = m_jsonImplementation->parse(source);
    }
    void JSON::parse(ISource &&source)
    {
        m_jNodeRoot = m_jsonImplementation->parse(source);
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON text in destination stream.
    /// </summary>
    /// <param name=destination>destination stream for stringified JSON</param>
    void JSON::stringify(IDestination &destination)
    {
        if (m_jNodeRoot == nullptr)
        {
            throw Error("No JSON to stringify.");
        }
        m_jsonImplementation->stringify(*m_jNodeRoot, destination);
    }
    void JSON::stringify(IDestination &&destination)
    {
        if (m_jNodeRoot == nullptr)
        {
            throw Error("No JSON to stringify.");
        }
        m_jsonImplementation->stringify(*m_jNodeRoot, destination);
    }
} // namespace JSONLib
