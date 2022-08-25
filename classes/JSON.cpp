//
// Class: JSON
//
// Description: Perform JSON  stringify/parse to/from a buffer or
// file. It is also  possible to customize this with the ISource
// and IDestination interfaces if required.For an in-depth description
// of the JSON specification refer to its RFC at web address
// https://tools.ietf.org/html/rfc8259 and JSON grammar at web page
// https://www.json.org/json-en.html.
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
/// <summary>
/// JSON constructor. Pass any custom converter or translator here.
/// </summary>
/// <param name="translator">Pointer to translator interface.</param>
/// <param name="converter">Pointer to converter interface.</param>
JSON::JSON(ITranslator *translator, IConverter *converter) : m_implementation(std::make_unique<JSON_Impl>())
{
  m_implementation->converter(converter);
  m_implementation->translator(translator);
}
/// <summary>
/// JSON constructor (array).
/// </summary>
/// <param name="array">Intialiser list of single values or JNode.</param>
JSON::JSON(const std::initializer_list<InternalTypes> &array) : JSON() { this->root() = JNode(array); }
/// <summary>
/// JSON constructor (object).
/// </summary>
/// <param name="array">Intialiser list of key/value(JNode) pairs.</param>
JSON::JSON(const std::initializer_list<std::pair<std::string, InternalTypes>> &object) : JSON()
{
  this->root() = JNode(object);
}
/// <summary>
/// JSON constructor. Pass a JSON string to be initially parsed.
/// </summary>
/// <param name="jsonString">JSON string.</param>
JSON::JSON(const std::string &jsonString) : JSON() { parse(BufferSource{ jsonString }); }
/// <summary>
/// JSON destructor.
/// </summary>
JSON::~JSON() {}
/// <summary>
/// Get JSONLib version.
/// </summary>
std::string JSON::version() const { return (m_implementation->version()); }
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON::strip(ISource &source, IDestination &destination) const { m_implementation->strip(source, destination); }
void JSON::strip(ISource &source, IDestination &&destination) const { m_implementation->strip(source, destination); }
void JSON::strip(ISource &&source, IDestination &destination) const { m_implementation->strip(source, destination); }
void JSON::strip(ISource &&source, IDestination &&destination) const { m_implementation->strip(source, destination); }
/// <summary>
/// Create JNode structure by parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
void JSON::parse(ISource &source) const { m_implementation->parse(source); }
void JSON::parse(ISource &&source) const { m_implementation->parse(source); }
/// <summary>
/// Traverse JNode structure and build its JSON text in destination stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON::stringify(IDestination &destination) const { m_implementation->stringify(destination); }
void JSON::stringify(IDestination &&destination) const { m_implementation->stringify(destination); }
/// <summary>
/// Recursively traverse JNode structure calling IAction methods or to change
/// the JSON tree node directly.
/// </summary>
/// <param name=action>Action methods to call during traversal.</param>
// Traverse using non-const JSON so can change JSON tree
void JSON::traverse(IAction &action) { m_implementation->traverse(action); }
// Traverse using const JSON so cannot change JSON tree
void JSON::traverse(IAction &action) const { std::as_const(*m_implementation).traverse(action); }
/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name=destination>Object entry (JNode) key.</param>
JNode &JSON::operator[](const std::string &key) { return ((*m_implementation)[key]); }
const JNode &JSON::operator[](const std::string &key) const { return ((*m_implementation)[key]); }
/// <summary>
/// Return array entry for the passed in index.
/// </summary>
/// <param name=destination>Array entry (JNode) index.</param>
JNode &JSON::operator[](std::size_t index) { return ((*m_implementation)[index]); }
const JNode &JSON::operator[](std::size_t index) const { return ((*m_implementation)[index]); }
/// <summary>
/// Return root of JSON tree.
/// </summary>
/// <returns>Root of JSON tree,</returns>
JNode &JSON::root() { return (m_implementation->root()); }
const JNode &JSON::root() const { return (m_implementation->root()); }
}// namespace JSONLib
