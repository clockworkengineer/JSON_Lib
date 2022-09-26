//
// Class: JSON_Impl
//
// Description: JSON class implementation layer that uses recursion to produce a
// JSON tree representation of JSON string data (parse).
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
// =================
// PRIVATE FUNCTIONS
// =================
/// <summary>
/// Extract a string from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
///  <param name="translator">String translator.</param>
/// <returns>Extracted string</returns>
std::string extractString(ISource &source, ITranslator &translator)
{
  bool translateEscapes = false;
  if (source.current() != '"') { throw Error(source.getPosition(), "Missing opening '\"' on string."); }
  source.next();
  std::string extracted;
  while (source.more() && source.current() != '"') {
    if (source.current() == '\\') {
      extracted += '\\';
      source.next();
      if (!translator.validEscape(source.current())) { extracted.pop_back(); }
      translateEscapes = true;
    }
    extracted += source.current();
    source.next();
  }
  if (source.current() != '"') { throw Error(source.getPosition(), "Missing closing '\"' on string."); }
  // Need to translate escapes to UTF8
  if (translateEscapes) { extracted = translator.fromJSON(extracted); }
  source.next();
  return (extracted);
}
/// <summary>
/// Has the end of a number been reached in source stream ?
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>==true on end of number</returns>
bool endOfNumber(ISource &source)
{
  return (source.isWS() || source.current() == ',' || source.current() == ']' || source.current() == '}');
}
// ===============
// PRIVATE METHODS
// ===============
/// <summary>
/// Parse a Object key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object key/value pair.</returns>
Object::Entry JSON_Impl::parseObjectEntry(ISource &source)
{
  source.ignoreWS();
  const std::string key{ extractString(source, *m_translator) };
  source.ignoreWS();
  if (source.current() != ':') { throw Error(source.getPosition(), "Missing ':' in key value pair."); }
  source.next();
  return (Object::Entry(key, parseJNodes(source)));
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>String JNode.</returns>
JNode JSON_Impl::parseString(ISource &source) { return (String::make(extractString(source, *m_translator))); }
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Number JNode.</returns>
JNode JSON_Impl::parseNumber(ISource &source)
{
  std::string string;
  for (; source.more() && !endOfNumber(source); source.next()) { string += source.current(); }
  Number number{ string };
  if (number.is<int>() || number.is<long>() || number.is<long long>() || number.is<float>() || number.is<double>()
      || number.is<long double>()) {
    return (Number::make(number));
  }
  throw Error(source.getPosition(), "Invalid numeric value.");
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Boolean JNode.</returns>
JNode JSON_Impl::parseBoolean(ISource &source)
{
  if (source.match("true")) { return (Boolean::make(true)); }
  if (source.match("false")) { return (Boolean::make(false)); }
  throw Error(source.getPosition(), "Invalid boolean value.");
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Null JNode.</returns>
JNode JSON_Impl::parseNull(ISource &source)
{
  if (!source.match("null")) { throw Error(source.getPosition(), "Invalid null value."); }
  return (Null::make());
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
    objectEntries.emplace_back(parseObjectEntry(source));
    while (source.current() == ',') {
      source.next();
      objectEntries.emplace_back(parseObjectEntry(source));
    }
  }
  if (source.current() != '}') { throw Error(source.getPosition(), "Missing closing '}' in object definition."); }
  source.next();
  return (Object::make(objectEntries));
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
  if (source.current() != ']') { throw Error(source.getPosition(), "Missing closing ']' in array definition."); }
  source.next();
  return (Array::make(array));
}
/// <summary>
/// Recursively parse JSON source stream producing a JNode structure
/// representation  of it. Note: If no obvious match is found for
/// parsing that it defaults to a numeric value.
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
  case '+':
  case '-':
    jNode = parseNumber(source);
    break;
  default:
    throw Error(source.getPosition(), "Missing String, Number, Boolean, Array, Object or Null.");
  }
  source.ignoreWS();
  return (jNode);
}
}// namespace JSON_Lib
