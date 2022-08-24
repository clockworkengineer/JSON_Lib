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
  if (source.current() != '"') { throw Error(source.getPosition(), "Missing opening '\"' on string."); }
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
  if (source.current() != '"') { throw Error(source.getPosition(), "Missing closing '\"' on string."); }
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
  if (source.current() != ':') { throw Error(source.getPosition(), "Missing ':' in key value pair."); }
  source.next();
  return (Object::Entry(key, parseJNodes(source)));
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>String JNode.</returns>
JNode JSON_Impl::parseString(ISource &source) { return (String::make(extractString(source))); }
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Number JNode.</returns>
JNode JSON_Impl::parseNumber(ISource &source)
{
  std::string number;
  for (; source.more() && Numeric::isNumericCharacter(source.current()); source.next()) { number += source.current(); }
  Numeric jNodeNumeric{ number };
  if (number.empty() || !jNodeNumeric.setValidNumber(number)) {
    throw Error(source.getPosition(), "Invalid numeric value.");
  }
  return (Number::make(jNodeNumeric));
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
    objectEntries.emplace_back(parseKeyValuePair(source));
    while (source.current() == ',') {
      source.next();
      objectEntries.emplace_back(parseKeyValuePair(source));
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
  default:
    jNode = parseNumber(source);
    break;
  }
  source.ignoreWS();
  return (jNode);
}
}// namespace JSONLib
