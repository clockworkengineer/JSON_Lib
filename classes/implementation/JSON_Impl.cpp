//
// Class: JSON_Impl
//
// Description: JSON class implementation layer.
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
/// <param name="source">Source for JSON encoded bytes.</param>
/// <param name="translate">== true and escapes found then they need
/// translating.</param> <returns>Extracted string</returns>
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
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns>Object key/value pair.</returns>
JNodeObjectEntry JSON_Impl::parseKeyValuePair(ISource &source) {
  const std::string keyValue{extractString(source)};
  if (source.current() != ':') {
    throw Error("Syntax error detected.");
  }
  source.next();
  source.ignoreWS();
  return (JNodeObjectEntry{keyValue, parseJNodes(source)});
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns>String value.</returns>
JNode::Ptr JSON_Impl::parseString(ISource &source) {
  return (makeJNodeString(extractString(source)));
}
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns></returns>
JNode::Ptr JSON_Impl::parseNumber(ISource &source) {
  JNodeNumber jsonNumber;
  for (int digits = 0;
       source.more() && jsonNumber.isValidNumeric(source.current());
       source.next()) {
    jsonNumber.number().value[digits++] = source.current();
    if (digits == JNodeNumeric::kLongLongWidth) {
      throw Error("Syntax error detected.");
    }
  }
  if (!jsonNumber.isValidNumber()) {
    throw Error("Syntax error detected.");
  }
  source.ignoreWS();
  return (makeJNodeNumber(jsonNumber.number()));
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns> Boolean JNode.</returns>
JNode::Ptr JSON_Impl::parseBoolean(ISource &source) {
  if (source.match("true")) {
    source.ignoreWS();
    return (makeJNodeBoolean(true));
  }
  if (source.match("false")) {
    source.ignoreWS();
    return (makeJNodeBoolean(false));
  }
  throw Error("Syntax error detected.");
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns>Null JNode.</returns>
JNode::Ptr JSON_Impl::parseNull(ISource &source) {
  if (!source.match("null")) {
    throw Error("Syntax error detected.");
  }
  source.ignoreWS();
  return (makeJNodeNull());
}
/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns>JNodeObject key/value pairs.</returns>
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
  return (makeJNodeObject(objects));
}
/// <summary>
/// Parse an array from a JSON source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns>JNodeArray.</returns>
JNode::Ptr JSON_Impl::parseArray(ISource &source) {
  std::vector<JNode::Ptr> array;
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
  return (makeJNodeArray(array));
}
/// <summary>
/// Recursively parse JSON source stream producing a JNode structure
/// representation  of it.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
/// <returns></returns>
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
/// <param name=jNode>JNode structure to be traversed</param>
/// <param name=destination>destination stream for stringified JSON</param>
/// <returns></returns>
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
/// <param name="source">Source of JSON</param>
/// <param name="destination">Destination for stripped JSON</param>
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
/// <param name="source">Source of JSON</param>
/// <param name="destination">Destination for stripped JSON</param>
void JSON_Impl::strip(ISource &source, IDestination &destination) {
  stripWhiteSpace(source, destination);
}
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.
JNode::Ptr JSON_Impl::parse(ISource &source) { return (parseJNodes(source)); }
/// <summary>
/// Create JNode structure by recursively parsing JSON on the source stream.
/// </summary>
/// <param name="jsonString">JSON string.
JNode::Ptr JSON_Impl::parse(const std::string &jsonString) {
  BufferSource source{jsonString};
  return (parse(source));
}
/// <summary>
/// Recursively parse JNode structure and building its JSON in destination
/// stream.
/// </summary>
/// <param name="jNodeRoot">Root of JNode structure.</param>
/// <param name=destination>destination stream for stringified JSON</param>
/// <returns></returns>
void JSON_Impl::stringify(const JNode &jNodeRoot, IDestination &destination) {
  stringifyJNodes(jNodeRoot, destination);
}
} // namespace JSONLib
