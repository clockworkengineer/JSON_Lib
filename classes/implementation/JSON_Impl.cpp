//
// Class: JSON_Impl
//
// Description: JSON class implementation layer that implementations recursive
// traversing to produce a JSON tree (parse) and also reconstitute the tree back 
// into raw JSON text (stringify).
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON_Impl.hpp"
#include "JSON.hpp"

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
std::string JSON_Impl::extractString(ISource &source, bool translate) {
  bool translateEscapes = false;
  if (source.current() != '"') {
    throw Error("Syntax error detected.");
  }
  source.next();
  std::string stringValue;
  while (source.more() && source.current() != '"') {
    if (source.current() == '\\') {
      stringValue += '\\';
      source.next();
      if (!translate && !m_translator->validEscape(source.current())) {
        stringValue.pop_back();
      }
      translateEscapes = translate;
    }
    stringValue += source.current();
    source.next();
  }
  if (source.current() != '"') {
    throw Error("Syntax error detected.");
  }
  source.next();
  source.ignoreWS();
  // Need to translate escapes to UTF8
  if (translateEscapes) {
    return (m_translator->fromJSON(stringValue));
  }
  // None so just pass on
  else {
    return (stringValue);
  }
}
/// <summary>
/// Parse a key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object key/value pair.</returns>
JNodeObject::ObjectEntry JSON_Impl::parseKeyValuePair(ISource &source) {
  const std::string keyValue{extractString(source)};
  if (source.current() != ':') {
    throw Error("Syntax error detected.");
  }
  source.next();
  source.ignoreWS();
  return (JNodeObject::ObjectEntry{keyValue, parseJNodes(source)});
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>String JNode.</returns>
JNode::Ptr JSON_Impl::parseString(ISource &source) {
  return (makeString(extractString(source)));
}
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Number JNode.</returns>
JNode::Ptr JSON_Impl::parseNumber(ISource &source) {
  std::string number;
  for (; source.more() && JNodeNumeric::isValidNumericChar(source.current());
       source.next()) {
    number += source.current();
  }
  JNodeNumeric jNodeNumeric{number};
  if (!jNodeNumeric.setValidNumber(number)) {
    throw Error("Syntax error detected.");
  }
  source.ignoreWS();
  return (makeNumber(jNodeNumeric));
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Boolean JNode.</returns>
JNode::Ptr JSON_Impl::parseBoolean(ISource &source) {
  if (source.match("true")) {
    source.ignoreWS();
    return (makeBoolean(true));
  }
  if (source.match("false")) {
    source.ignoreWS();
    return (makeBoolean(false));
  }
  throw Error("Syntax error detected.");
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Null JNode.</returns>
JNode::Ptr JSON_Impl::parseNull(ISource &source) {
  if (!source.match("null")) {
    throw Error("Syntax error detected.");
  }
  source.ignoreWS();
  return (makeNull());
}
/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object JNode (key/value pairs).</returns>
JNode::Ptr JSON_Impl::parseObject(ISource &source) {
  JNodeObject::ObjectList objects;
  source.next();
  source.ignoreWS();
  if (source.current() != '}') {
    objects.emplace_back(parseKeyValuePair(source));
    while (source.current() == ',') {
      source.next();
      source.ignoreWS();
      objects.emplace_back(parseKeyValuePair(source));
    }
  }
  if (source.current() != '}') {
    throw Error("Syntax error detected.");
  }
  source.next();
  source.ignoreWS();
  return (makeObject(objects));
}
/// <summary>
/// Parse an array from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Array JNode.</returns>
JNode::Ptr JSON_Impl::parseArray(ISource &source) {
  JNodeArray::ArrayList array;
  source.next();
  source.ignoreWS();
  if (source.current() != ']') {
    array.emplace_back(parseJNodes(source));
    while (source.current() == ',') {
      source.next();
      source.ignoreWS();
      array.emplace_back(parseJNodes(source));
    }
  }
  if (source.current() != ']') {
    throw Error("Syntax error detected.");
  }
  source.next();
  source.ignoreWS();
  return (makeArray(array));
}
/// <summary>
/// Recursively parse JSON source stream producing a JNode structure
/// representation  of it.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Pointer to JNode.</returns>
JNode::Ptr JSON_Impl::parseJNodes(ISource &source) {
  source.ignoreWS();
  switch (source.current()) {
  case '"':
    return (parseString(source));
  case 't':
  case 'f':
    return (parseBoolean(source));
  case 'n':
    return (parseNull(source));
  case '{':
    return (parseObject(source));
  case '[':
    return (parseArray(source));
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
    return (parseNumber(source));
  }
  throw Error("Syntax error detected.");
}
/// <summary>
/// Recursively traverse JNode structure encoding it into JSON on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringifyJNodes(const JNode &jNode, IDestination &destination) {
  switch (jNode.getNodeType()) {
  case JNodeType::number:
    destination.add(JNodeRef<const JNodeNumber>(jNode).toString());
    break;
  case JNodeType::string:
    destination.add('"');
    destination.add(
        m_translator->toJSON(JNodeRef<JNodeString>(jNode).toString()));
    destination.add('"');
    break;
  case JNodeType::boolean:
    destination.add(JNodeRef<JNodeBoolean>(jNode).toString());
    break;
  case JNodeType::null:
    destination.add(JNodeRef<JNodeNull>(jNode).toString());
    break;
  case JNodeType::hole:
    destination.add(JNodeRef<JNodeHole>(jNode).toString());
    break;
  case JNodeType::object: {
    int commaCount = JNodeRef<JNodeObject>(jNode).size() - 1;
    destination.add('{');
    for (auto &[key, jNodePtr] : JNodeRef<JNodeObject>(jNode).objects()) {
      destination.add('"');
      destination.add(m_translator->toJSON(key));
      destination.add("\":");
      stringifyJNodes(JNodeRef<JNodeObject>(jNode)[key], destination);
      if (commaCount-- > 0) {
        destination.add(',');
      }
    }
    destination.add('}');
    break;
  }
  case JNodeType::array: {
    std::size_t commaCount = JNodeRef<JNodeArray>(jNode).size() - 1;
    destination.add('[');
    for (auto &bNodeEntry : JNodeRef<JNodeArray>(jNode).array()) {
      stringifyJNodes(*bNodeEntry, destination);
      if (commaCount-- > 0) {
        destination.add(',');
      }
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
void JSON_Impl::stripWhiteSpace(ISource &source, IDestination &destination) {
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
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
///  Get JSONLib version.
/// </summary>
std::string JSON_Impl::version() {
  std::stringstream versionString;
  versionString << "JSONLib Version  " << JSON_VERSION_MAJOR << "."
                << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
  return (versionString.str());
}
/// <summary>
/// Set translator for JSON strings.
/// </summary>
/// <param name=translator>Custom JSON string translator.</param>
void JSON_Impl::translator(ITranslator *translator) {
  if (translator == nullptr) {
    m_translator = std::make_unique<JSON_Translator>(*m_converter);
  } else {
    m_translator.reset(translator);
  }
}
/// <summary>
/// Set converter for JSON strings.
/// </summary>
/// <param name=conv>Custom JSON string translator.</param>
void JSON_Impl::converter(IConverter *converter) {
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
void JSON_Impl::strip(ISource &source, IDestination &destination) {
  stripWhiteSpace(source, destination);
}
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
void JSON_Impl::parse(ISource &source) { m_jNodeRoot = parseJNodes(source); }
/// <summary>
/// Create JNode structure by recursively parsing JSON string passed.
/// </summary>
/// <param name="jsonString">JSON string.</param>
void JSON_Impl::parse(const std::string &jsonString) {
  BufferSource source{jsonString};
  parse(source);
}
/// <summary>
/// Recursively parse JNode structure and building its JSON in destination
/// stream.
/// </summary>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <returns></returns>
void JSON_Impl::stringify(IDestination &destination) {
  if (m_jNodeRoot == nullptr) {
    throw Error("No JSON to stringify.");
  }
  stringifyJNodes(*m_jNodeRoot, destination);
}
/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name=key>Object entry (JNode) key.</param>
JNode &JSON_Impl::operator[](const std::string &key) {
  try {
    if (m_jNodeRoot == nullptr) {
      parse("{}");
    }
    return ((*m_jNodeRoot)[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JNodeRef<JNodeObject>(*m_jNodeRoot)
        .objects()
        .emplace_back(JNodeObject::ObjectEntry{key, makeHole()});
    return (*JNodeRef<JNodeObject>(*m_jNodeRoot).objects().back().value);
  }
}
const JNode &JSON_Impl::operator[](const std::string &key) const // Object
{
  return ((*m_jNodeRoot)[key]);
}
/// <summary>
/// Return array entry for the passed in index.
/// </summary>
/// <param name=index>Array entry (JNode) index.</param>
JNode &JSON_Impl::operator[](std::size_t index) {
  try {
    if (m_jNodeRoot == nullptr) {
      parse("[]");
    }
    return ((*m_jNodeRoot)[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JNodeRef<JNodeArray>(*m_jNodeRoot).array().resize(index + 1);
    JNodeRef<JNodeArray>(*m_jNodeRoot).array()[index] = std::move(makeNull());
    return (*JNodeRef<JNodeArray>(*m_jNodeRoot).array()[index]);
  }
}
const JNode &JSON_Impl::operator[](std::size_t index) const {
  return ((*m_jNodeRoot)[index]);
}
} // namespace JSONLib
