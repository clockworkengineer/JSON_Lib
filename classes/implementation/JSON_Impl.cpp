//
// Class: JSON_Impl
//
// Description: JSON class implementation layer that uses recursion to produce a
// JSON tree (parse) and also reconstitute the tree back into raw JSON text
// (stringify).
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
/// <summary>
/// Extract a string from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translate">== true and escapes found then they need
/// translating.</param>
/// <returns>Extracted string</returns>
std::string JSON_Impl::extractString(ISource &source, bool translate)
{
  bool translateEscapes = false;
  if (source.current() != '"') { throw Error("Syntax error detected."); }
  source.next();
  std::string extracted;
  while (source.more() && source.current() != '"') {
    if (source.current() == '\\') {
      extracted += '\\';
      source.next();
      if (!translate && !m_translator->validEscape(source.current())) { extracted.pop_back(); }
      translateEscapes = translate;
    }
    extracted += source.current();
    source.next();
  }
  if (source.current() != '"') { throw Error("Syntax error detected."); }
  source.next();
  // Need to translate escapes to UTF8
  if (translateEscapes) {
    return (m_translator->fromJSON(extracted));
  }
  // None so just pass on
  else {
    return (extracted);
  }
}
/// <summary>
/// Parse a key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object key/value pair.</returns>
Object::Entry JSON_Impl::parseKeyValuePair(ISource &source)
{
  source.ignoreWS();
  const std::string key{ extractString(source) };
  source.ignoreWS();
  if (source.current() != ':') { throw Error("Syntax error detected."); }
  source.next();
  return (Object::Entry(key, parseJNodes(source)));
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>String JNode.</returns>
JNode JSON_Impl::parseString(ISource &source) { return (makeString(extractString(source))); }
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Number JNode.</returns>
JNode JSON_Impl::parseNumber(ISource &source)
{
  std::string number;
  for (; source.more() && Numeric::isValidNumericChar(source.current()); source.next()) { number += source.current(); }
  Numeric jNodeNumeric{ number };
  if (!jNodeNumeric.setValidNumber(number)) { throw Error("Syntax error detected."); }
  return (makeNumber(jNodeNumeric));
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Boolean JNode.</returns>
JNode JSON_Impl::parseBoolean(ISource &source)
{
  if (source.match("true")) { return (makeBoolean(true)); }
  if (source.match("false")) { return (makeBoolean(false)); }
  throw Error("Syntax error detected.");
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Null JNode.</returns>
JNode JSON_Impl::parseNull(ISource &source)
{
  if (!source.match("null")) { throw Error("Syntax error detected."); }
  return (makeNull());
}
/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object JNode (key/value pairs).</returns>
JNode JSON_Impl::parseObject(ISource &source)
{
  Object::EntryList objectEntries;
  source.next();
  source.ignoreWS();
  if (source.current() != '}') {
    objectEntries.emplace_back(parseKeyValuePair(source));
    while (source.current() == ',') {
      source.next();
      objectEntries.emplace_back(parseKeyValuePair(source));
    }
  }
  if (source.current() != '}') { throw Error("Syntax error detected."); }
  source.next();
  return (makeObject(objectEntries));
}
/// <summary>
/// Parse an array from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Array JNode.</returns>
JNode JSON_Impl::parseArray(ISource &source)
{
  Array::ArrayList array;
  source.next();
  source.ignoreWS();
  if (source.current() != ']') {
    array.emplace_back(parseJNodes(source));
    while (source.current() == ',') {
      source.next();
      array.emplace_back(parseJNodes(source));
    }
  }
  if (source.current() != ']') { throw Error("Syntax error detected."); }
  source.next();
  return (makeArray(array));
}
/// <summary>
/// Recursively parse JSON source stream producing a JNode structure
/// representation  of it.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Pointer to JNode.</returns>
JNode JSON_Impl::parseJNodes(ISource &source)
{
  JNode jNode;
  source.ignoreWS();
  switch (source.current()) {
  case '{':
    jNode = parseObject(source);
    break;
  case '[':
    jNode = parseArray(source);
    break;
  case '"':
    jNode = parseString(source);
    break;
  case 't':
  case 'f':
    jNode = parseBoolean(source);
    break;
  case 'n':
    jNode = parseNull(source);
    break;
  case '-':
  case '+':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    jNode = parseNumber(source);
    break;
  default:
    throw Error("Syntax error detected.");
  }
  source.ignoreWS();
  return (jNode);
}
/// <summary>
/// Recursively traverse JNode structure encoding it into JSON text on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringifyJNodes(const JNode &jNode, IDestination &destination)
{
  switch (jNode.getType()) {
  case JNodeType::number:
    destination.add(JRef<Number>(jNode).toString());
    break;
  case JNodeType::string:
    destination.add('"');
    destination.add(m_translator->toJSON(JRef<String>(jNode).toString()));
    destination.add('"');
    break;
  case JNodeType::boolean:
    destination.add(JRef<Boolean>(jNode).toString());
    break;
  case JNodeType::null:
    destination.add(JRef<Null>(jNode).toString());
    break;
  case JNodeType::hole:
    destination.add(JRef<Hole>(jNode).toString());
    break;
  case JNodeType::object: {
    int commaCount = JRef<Object>(jNode).size() - 1;
    destination.add('{');
    for (auto &entry : JRef<Object>(jNode).objectEntries()) {
      destination.add('"');
      destination.add(m_translator->toJSON(entry.getKey()));
      destination.add("\":");
      stringifyJNodes(entry.getJNode(), destination);
      if (commaCount-- > 0) { destination.add(','); }
    }
    destination.add('}');
    break;
  }
  case JNodeType::array: {
    std::size_t commaCount = JRef<Array>(jNode).size() - 1;
    destination.add('[');
    for (auto &node : JRef<Array>(jNode).array()) {
      stringifyJNodes(node, destination);
      if (commaCount-- > 0) { destination.add(','); }
    }
    destination.add(']');
    break;
  }
  default:
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON_Impl::stripWhiteSpace(ISource &source, IDestination &destination)
{
  while (source.more()) {
    if (!source.isWS()) {
      destination.add(source.current());
      if (source.current() == '"') {
        destination.add(extractString(source, false));
        destination.add('"');
        continue;
      }
    }
    source.next();
  }
}
/// <summary>
/// Recursively traverse JNode tree calling IAction methods and possibly
/// modifying the tree contents or even structure.
/// </summary>
/// <param name=jNode>JNode tree to be traversed.</param>
/// <param name=action>Action methods to call during traversal.</param>
void JSON_Impl::traverseJNodes(JNode &jNode, IAction &action)
{
  switch (jNode.getType()) {
  case JNodeType::number:
    action.onNumber(JRef<Number>(jNode));
    break;
  case JNodeType::string:
    action.onString(JRef<String>(jNode));
    break;
  case JNodeType::boolean:
    action.onBoolean(JRef<Boolean>(jNode));
    break;
  case JNodeType::null:
    action.onNull(JRef<Null>(jNode));
    break;
  case JNodeType::hole:
    break;
  case JNodeType::object: {
    action.onObject(JRef<Object>(jNode));
    for (auto &entry : JRef<Object>(jNode).objectEntries()) { traverseJNodes(entry.getJNode(), action); }
    break;
  }
  case JNodeType::array: {
    action.onArray(JRef<Array>(jNode));
    for (auto &node : JRef<Array>(jNode).array()) { traverseJNodes(node, action); }
    break;
  }
  default:
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
void JSON_Impl::traverseJNodes(const JNode &jNode, IAction &action)
{
  switch (jNode.getType()) {
  case JNodeType::number:
    action.onNumber(JRef<Number>(jNode));
    break;
  case JNodeType::string:
    action.onString(JRef<String>(jNode));
    break;
  case JNodeType::boolean:
    action.onBoolean(JRef<Boolean>(jNode));
    break;
  case JNodeType::null:
    action.onNull(JRef<Null>(jNode));
    break;
  case JNodeType::hole:
    break;
  case JNodeType::object: {
    action.onObject(JRef<Object>(jNode));
    for (auto &entry : JRef<Object>(jNode).objectEntries()) { traverseJNodes(entry.getJNode(), action); }
    break;
  }
  case JNodeType::array: {
    action.onArray(JRef<Array>(jNode));
    for (auto &node : JRef<Array>(jNode).array()) { traverseJNodes(node, action); }
    break;
  }
  default:
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
///  Get JSONLib version.
/// </summary>
std::string JSON_Impl::version() const
{
  std::stringstream versionString;
  versionString << "JSONLib Version  " << JSON_VERSION_MAJOR << "." << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
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
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON_Impl::strip(ISource &source, IDestination &destination) const { stripWhiteSpace(source, destination); }
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
void JSON_Impl::parse(ISource &source) { m_jNodeRoot = parseJNodes(source); }
/// <summary>
/// Create JNode structure by recursively parsing JSON string passed.
/// </summary>
/// <param name="jsonString">JSON string.</param>
void JSON_Impl::parse(const std::string &jsonString)
{
  BufferSource source{ jsonString };
  parse(source);
}
/// <summary>
/// Recursively parse JNode structure and building its JSON in destination
/// stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringify(IDestination &destination) const
{
  if (m_jNodeRoot.getVariant() == nullptr) { throw Error("No JSON to stringify."); }
  stringifyJNodes(m_jNodeRoot, destination);
}
/// <summary>
/// Recursively traverse JNode structure calling IAction methods.
/// </summary>
/// <param name=action>Action methods to call during traversal.</param>
void JSON_Impl::traverse(IAction &action)
{
  if (m_jNodeRoot.getVariant() == nullptr) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (m_jNodeRoot.getVariant() == nullptr) { throw Error("No JSON to traverse."); }
  traverseJNodes(m_jNodeRoot, action);
}
/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name=key>Object entry (JNode) key.</param>
JNode &JSON_Impl::operator[](const std::string &key)
{
  try {
    if (m_jNodeRoot.getVariant() == nullptr) { parse("{}"); }
    return ((m_jNodeRoot)[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Object>(m_jNodeRoot).objectEntries().emplace_back(Object::Entry(key, makeHole()));
    return (JRef<Object>(m_jNodeRoot).objectEntries().back().getJNode());
  }
}
const JNode &JSON_Impl::operator[](const std::string &key) const// Object
{
  return ((m_jNodeRoot)[key]);
}
/// <summary>
/// Return array entry for the passed in index.
/// </summary>
/// <param name=index>Array entry (JNode) index.</param>
JNode &JSON_Impl::operator[](std::size_t index)
{
  try {
    if (m_jNodeRoot.getVariant() == nullptr) { parse("[]"); }
    return ((m_jNodeRoot)[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Array>(m_jNodeRoot).array().resize(index + 1);
    JRef<Array>(m_jNodeRoot).array()[index] = std::move(makeNull());
    return (JRef<Array>(m_jNodeRoot).array()[index]);
  }
}
const JNode &JSON_Impl::operator[](std::size_t index) const { return ((m_jNodeRoot)[index]); }
}// namespace JSONLib
