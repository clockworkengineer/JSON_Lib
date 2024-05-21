//
// Class: JSON_Parser
//
// Description:
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Parser.hpp"

namespace JSON_Lib {

/// <summary>
/// Check whether character is avalid escaped character or is just
/// normal escaped ASCII character. Only a few characters are valid
/// escaped characters such as '\t' or '\"' but normal ASCII characters
/// may still be can still have a '\' prefix and be escaped though not
/// proper escaped sense.
/// </summary>
/// <param name="escape">Escaped character.</param>
/// <returns>==true then character is a valid escape character.</returns>
bool validEscape(const char escape)
{
  return escape == '\\' || escape == 't' || escape == '"' || escape == 'b' || escape == 'f' || escape == 'n'
         || escape == 'r' || escape == 'u';
}

/// <summary>
/// Extract a string from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
///  <param name="translator">String translator.</param>
/// <returns>Extracted string</returns>
std::string extractString(ISource &source, ITranslator &translator)
{
  bool translateEscapes = false;
  if (source.current() != '"') { throw SyntaxError(source.getPosition(), "Missing opening '\"' on string."); }
  source.next();
  std::string extracted;
  while (source.more() && source.current() != '"') {
    if (source.current() == '\\') {
      extracted += '\\';
      source.next();
      if (!validEscape(source.current())) { extracted.pop_back(); }
      translateEscapes = true;
    }
    extracted += source.current();
    source.next();
  }
  if (source.current() != '"') { throw SyntaxError(source.getPosition(), "Missing closing '\"' on string."); }
  // Need to translate escapes to UTF8
  if (translateEscapes) { extracted = translator.from(extracted); }
  source.next();
  return extracted;
}

/// <summary>
/// Has the end of a number been reached in source stream ?
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>==true on end of number</returns>
bool endOfNumber(ISource &source)
{
  return source.isWS() || source.current() == ',' || source.current() == ']' || source.current() == '}';
}

/// <summary>
/// Parse a Object key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object key/value pair.</returns>
Object::Entry JSON_Parser::parseObjectEntry(ISource &source)
{
  source.ignoreWS();
  const std::string key{ extractString(source, jsonTranslator) };
  source.ignoreWS();
  if (source.current() != ':') { throw SyntaxError(source.getPosition(), "Missing ':' in key value pair."); }
  source.next();
  return Object::Entry(key, parse(source));
}

/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>String JNode.</returns>
JNode JSON_Parser::parseString(ISource &source) const
{ return JNode::make<String>(extractString(source, jsonTranslator)); }

/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Number JNode.</returns>
JNode JSON_Parser::parseNumber(ISource &source)
{
  std::string string;
  for (; source.more() && !endOfNumber(source); source.next()) { string += source.current(); }
  Number number{ string };
  if (number.is<int>() || number.is<long>() || number.is<long long>() || number.is<float>() || number.is<double>()
      || number.is<long double>()) {
    return JNode::make<Number>(number);
  }
  throw SyntaxError(source.getPosition(), "Invalid numeric value.");
}

/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Boolean JNode.</returns>
JNode JSON_Parser::parseBoolean(ISource &source)
{
  if (source.match("true")) { return JNode::make<Boolean>(true); }
  if (source.match("false")) { return JNode::make<Boolean>(false); }
  throw SyntaxError(source.getPosition(), "Invalid boolean value.");
}

/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Null JNode.</returns>
JNode JSON_Parser::parseNull(ISource &source)
{
  if (!source.match("null")) { throw SyntaxError(source.getPosition(), "Invalid null value."); }
  return JNode::make<Null>();
}

/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Object JNode (key/value pairs).</returns>
JNode JSON_Parser::parseObject(ISource &source)
{
  JNode jNodeObject = JNode::make<Object>();
  source.next();
  source.ignoreWS();
  if (source.current() != '}') {
    JRef<Object>(jNodeObject).add(parseObjectEntry(source));
    while (source.current() == ',') {
      source.next();
      JRef<Object>(jNodeObject).add(parseObjectEntry(source));
    }
  }
  if (source.current() != '}') { throw SyntaxError(source.getPosition(), "Missing closing '}' in object definition."); }
  source.next();
  return jNodeObject;
}

/// <summary>
/// Parse an array from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Array JNode.</returns>
JNode JSON_Parser::parseArray(ISource &source)
{
  JNode jNodeArray = JNode::make<Array>();
  source.next();
  source.ignoreWS();
  if (source.current() != ']') {
    JRef<Array>(jNodeArray).add(parse(source));
    while (source.current() == ',') {
      source.next();
      JRef<Array>(jNodeArray).add(parse(source));
    }
  }
  if (source.current() != ']') { throw SyntaxError(source.getPosition(), "Missing closing ']' in array definition."); }
  source.next();
  return jNodeArray;
}

/// <summary>
/// Recursively parse JSON source stream producing a JNode structure
/// representation  of it. Note: If no obvious match is found for
/// parsing that it defaults to a numeric value.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Pointer to JNode.</returns>
JNode JSON_Parser::parse(ISource &source)
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
    throw SyntaxError(source.getPosition(), "Missing String, Number, Boolean, Array, Object or Null.");
  }
  source.ignoreWS();
  return jNode;
}

}// namespace JSON_Lib
