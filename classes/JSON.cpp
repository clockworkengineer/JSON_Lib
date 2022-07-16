//
// Class: JSON
//
// Description: Perform JSON  stringify/parse to/from a buffer or
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
/// JSON constructor. Pass any custom convert or translator here.
/// </summary>
/// <param name="translator">Pointer to translator interface.</param>
/// <param name="converter">Pointer to converter interface.</param>
JSON::JSON(ITranslator *translator, IConverter *converter)
    : m_jsonImplementation(std::make_unique<JSON_Impl>()) {
  m_jsonImplementation->converter(converter);
  m_jsonImplementation->translator(translator);
}
/// <summary>
/// JSON constructor. Pass a JSON string to be initially parsed.
/// </summary>
/// <param name="jsonString">JSON string.</param>
JSON::JSON(const std::string &jsonString) : JSON() {
  m_jNodeRoot = m_jsonImplementation->parse(jsonString);
}
/// <summary>
/// JSON destructor.
/// </summary>
JSON::~JSON() {}
/// <summary>
/// Get JSONLib version.
/// </summary>
std::string JSON::version() { return (m_jsonImplementation->version()); }
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON::strip(ISource &source, IDestination &destination) {
  m_jsonImplementation->strip(source, destination);
}
void JSON::strip(ISource &source, IDestination &&destination) {
  m_jsonImplementation->strip(source, destination);
}
void JSON::strip(ISource &&source, IDestination &destination) {
  m_jsonImplementation->strip(source, destination);
}
void JSON::strip(ISource &&source, IDestination &&destination) {
  m_jsonImplementation->strip(source, destination);
}
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.
void JSON::parse(ISource &source) {
  m_jNodeRoot = m_jsonImplementation->parse(source);
}
void JSON::parse(ISource &&source) {
  m_jNodeRoot = m_jsonImplementation->parse(source);
}
/// <summary>
/// Recursively parse JNode structure and building its JSON text in destination
/// stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON::stringify(IDestination &destination) {
  if (m_jNodeRoot == nullptr) {
    throw Error("No JSON to stringify.");
  }
  m_jsonImplementation->stringify(*m_jNodeRoot, destination);
}
void JSON::stringify(IDestination &&destination) {
  if (m_jNodeRoot == nullptr) {
    throw Error("No JSON to stringify.");
  }
  m_jsonImplementation->stringify(*m_jNodeRoot, destination);
}
/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name=destination>Object entry (JNode) key.</param>
JNode &JSON::operator[](const std::string &key) {
  try {
    if (m_jNodeRoot == nullptr) {
      m_jNodeRoot = m_jsonImplementation->parse("{}");
    }
    return ((*m_jNodeRoot)[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JNodeRef<JNodeObject>(*m_jNodeRoot)
        .objects()
        .emplace_back(JNodeObject::ObjectEntry{key, makeHole()});
    return (*JNodeRef<JNodeObject>(*m_jNodeRoot).objects().back().value);
  }
}
const JNode &JSON::operator[](const std::string &key) const // Object
{
  return ((*m_jNodeRoot)[key]);
}
/// <summary>
/// Return array entry for the passed in index.
/// </summary>
/// <param name=destination>Array entry (JNode)index.</param>
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
