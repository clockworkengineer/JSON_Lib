//
// Class: JSON_Impl
//
// Description: High level entry points in JSON class implementation layer.
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
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
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
///  Get JSON_Lib version.
/// </summary>
std::string JSON_Impl::version() const
{
  std::stringstream versionString;
  versionString << "JSON_Lib Version  " << JSON_VERSION_MAJOR << "." << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
  return (versionString.str());
}
/// <summary>
/// Set translator for JSON strings.
/// </summary>
/// <param name=translator>Custom JSON string translator.</param>
void JSON_Impl::translator(ITranslator *translator)
{
  if (translator == nullptr) {
    m_translator = std::make_unique<JSON_Translator>(*m_converter);
  } else {
    m_translator.reset(translator);
  }
}
/// <summary>
/// Set converter for JSON strings.
/// </summary>
/// <param name=converter>Custom JSON string translator.</param>
void JSON_Impl::converter(IConverter *converter)
{
  if (converter == nullptr) {
    m_converter = std::make_unique<JSON_Converter>();
  } else {
    m_converter.reset(converter);
  }
}
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
void JSON_Impl::parse(ISource &source) { m_jNodeRoot = parseJNodes(source); }
/// <summary>
/// Recursively parse JNode tree structure and building its JSON string (no whitespace)
/// in destination stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringify(IDestination &destination) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to stringify."); }
  stringifyJNodes(m_jNodeRoot, destination, 0);
}
/// <summary>
/// Recursively parse JNode tree structure and building its JSON string (pretty printed)
/// in destination stream.
/// </summary>
/// <param name=destination>Destination stream for printed JSON.</param>
void JSON_Impl::print(IDestination &destination) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to print."); }
  stringifyJNodes(m_jNodeRoot, destination, m_indent);
}
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON_Impl::strip(ISource &source, IDestination &destination) const { stripWhitespace(source, destination); }
/// <summary>
/// Recursively traverse JNode structure calling IAction methods.
/// </summary>
/// <param name=action>Action methods to call during traversal.</param>
void JSON_Impl::traverse(IAction &action)
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (m_jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
/// <summary>
/// Return object entry for the passed in key creating a root object if it does not exist
/// or a placeholder for a new JNode to be created into if the key does not exist.
/// </summary>
/// <param name=key>Object entry (JNode) key.</param>
JNode &JSON_Impl::operator[](const std::string &key)
{
  try {
    if (m_jNodeRoot.isEmpty()) { m_jNodeRoot = Object::make(); }
    return (m_jNodeRoot[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Object>(m_jNodeRoot).getObjectEntries().emplace_back(Object::Entry(key, Hole::make()));
    return (m_jNodeRoot[key]);
  }
}
const JNode &JSON_Impl::operator[](const std::string &key) const// Objec
{
  return ((m_jNodeRoot)[key]);
}
/// <summary>
/// Return array entry for the passed in index creating a root array if it does not exist
/// or a placeholder(s) for a new JNode to be created into if the index(s) do not exist.
/// </summary>
/// <param name=index>Array entry (JNode) index.</param>
JNode &JSON_Impl::operator[](std::size_t index)
{
  try {
    if (m_jNodeRoot.isEmpty()) { m_jNodeRoot = Array::make(); }
    return (m_jNodeRoot[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Array>(m_jNodeRoot).resize(index);
    return (m_jNodeRoot[index]);
  }
}
const JNode &JSON_Impl::operator[](std::size_t index) const { return ((m_jNodeRoot)[index]); }
}// namespace JSON_Lib
