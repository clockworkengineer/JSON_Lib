//
// Class: JSON_Parser
//
// Description:
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

/// <summary>
/// Check whether character is a valid-escaped character or is just
/// normally escaped ASCII character. Only a few characters are valid
/// escaped characters such as '\t' or '\"' but normal ASCII characters
/// may still be can still have a '\' prefix and be escaped though not
/// proper escaped sense.
/// </summary>
/// <param name="escape">Escaped character.</param>
/// <returns>True then character is a valid escape character.</returns>
bool validEscape(const char escape)
{
  return escape == '\\' || escape == 't' || escape == '"' || escape == 'b' || escape == 'f' || escape == 'n'
         || escape == 'r' || escape == 'u';
}
/// <summary>
/// Extract a string from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <returns>Extracted string</returns>
std::string extractString(ISource &source, const ITranslator &translator)
{
  uint64_t stringLength = 0;
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
    stringLength++;
    if (stringLength>String::getMaxStringLength()) {
      throw SyntaxError("String size exceeds maximum allowed size.");
    }
    source.next();
  }
  if (source.current() != '"') { throw SyntaxError(source.getPosition(), "Missing closing '\"' on string."); }
  // Need to translate escapes to UTF8
  if (translateEscapes) { extracted = translator.from(extracted); }
  source.next();
  return extracted;
}
/// <summary>
/// Has the end of a number been reached in source stream?
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>True on end of number</returns>
bool endOfNumber(const ISource &source)
{
  return source.isWS() || source.current() == ',' || source.current() == ']' || source.current() == '}';
}
/// <summary>
/// Parse an Object key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Object key/value pair.</returns>
Object::Entry Default_Parser::parseObjectEntry(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  source.ignoreWS();
  const std::string key{ extractString(source, translator) };
  source.ignoreWS();
  if (source.current() != ':') { throw SyntaxError(source.getPosition(), "Missing ':' in key value pair."); }
  source.next();
  return {key, parseNodes(source, translator, parserDepth + 1)};
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>String Node.</returns>
Node Default_Parser::parseString(ISource &source, const ITranslator &translator,[[maybe_unused]] const unsigned long parserDepth)
{
  return Node::make<String>(extractString(source, translator));
}
/// <summary>
/// Parse a number from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Number Node.</returns>
Node Default_Parser::parseNumber(ISource &source, [[maybe_unused]]const ITranslator &translator, [[maybe_unused]]const unsigned long parserDepth)
{
  std::string string;
  for (; source.more() && !endOfNumber(source); source.next()) { string += source.current(); }
  if (Number number{ string }; number.is<int>() || number.is<long>() || number.is<long long>() || number.is<float>()
                               || number.is<double>() || number.is<long double>()) {
    return Node::make<Number>(number);
  }
  throw SyntaxError(source.getPosition(), "Invalid numeric value.");
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Boolean Node.</returns>
Node Default_Parser::parseBoolean(ISource &source,  [[maybe_unused]]const ITranslator &translator,[[maybe_unused]] const unsigned long parserDepth)
{
  if (source.match("true")) { return Node::make<Boolean>(true); }
  if (source.match("false")) { return Node::make<Boolean>(false); }
  throw SyntaxError(source.getPosition(), "Invalid boolean value.");
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Null Node.</returns>
Node Default_Parser::parseNull(ISource &source,  [[maybe_unused]]const ITranslator &translator, [[maybe_unused]]const unsigned long parserDepth)
{
  if (!source.match("null")) { throw SyntaxError(source.getPosition(), "Invalid null value."); }
  return Node::make<Null>();
}
/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Object Node (key/value pairs).</returns>
Node Default_Parser::parseObject(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  Node jNodeObject = Node::make<Object>();
  source.next();
  source.ignoreWS();
  if (source.current() != '}') {
    JRef<Object>(jNodeObject).add(parseObjectEntry(source, translator,parserDepth));
    while (source.current() == ',') {
      source.next();
      JRef<Object>(jNodeObject).add(parseObjectEntry(source, translator, parserDepth));
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
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Array Node.</returns>
Node Default_Parser::parseArray(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  Node jNodeArray = Node::make<Array>();
  source.next();
  source.ignoreWS();
  if (source.current() != ']') {
    JRef<Array>(jNodeArray).add(parseNodes(source, translator,parserDepth+1));
    while (source.current() == ',') {
      source.next();
      JRef<Array>(jNodeArray).add(parseNodes(source, translator, parserDepth+1));
    }
  }
  if (source.current() != ']') { throw SyntaxError(source.getPosition(), "Missing closing ']' in array definition."); }
  source.next();
  return jNodeArray;
}
/// <summary>
/// Recursively parse JSON source stream producing a Node structure
/// representation  of it. Note: If no obvious match is found for
/// parsing that it defaults to a numeric value.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Pointer to Node.</returns>
Node Default_Parser::parseNodes(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  if (parserDepth>=getMaxParserDepth()) {
    throw SyntaxError("Maximum parser depth exceeded.");
  }
  source.ignoreWS();
  const auto it = parsers.find(source.current());
  if (it == parsers.end()) {
    throw SyntaxError(source.getPosition(), "Missing String, Number, Boolean, Array, Object or Null.");
  }
  Node jNode = it->second(source, translator,  parserDepth);
  source.ignoreWS();
  return jNode;
}
/// <summary>
/// Parse JSON source stream producing a Node structure representation  of it. Note: If no obvious match
/// is found for parsing that it defaults to a numeric value.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <returns>Pointer to Node.</returns>
Node Default_Parser::parse(ISource &source) { return parseNodes(source, jsonTranslator, 1); }
}// namespace JSON_Lib
