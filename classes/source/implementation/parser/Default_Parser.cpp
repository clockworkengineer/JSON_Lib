//
// Class: JSON_Parser
//
// Description:
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Throw.hpp"
#include <array>
#include <string_view>

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
  return escape == JSON_Lib::kEscape || escape == 't' || escape == JSON_Lib::kStringQuote || escape == 'b'
         || escape == 'f' || escape == 'n' || escape == 'r' || escape == 'u';
}
/// <summary>
/// Extract a string from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <returns>Extracted string</returns>
String extractString(ISource &source, const ITranslator &translator)
{
  uint64_t stringLength = 0;
  bool translateEscapes = false;
  if (source.current() != '"') { JSON_THROW(SyntaxError(source.getPosition(), "Missing opening '\"' on string.")); }
  source.next();
  String extracted;
  extracted.reserve(64);
  while (source.more() && source.current() != JSON_Lib::kStringQuote) {
    if (source.current() == '\\') {
      extracted.append('\\');
      source.next();
      if (!validEscape(source.current())) { extracted.pop_back(); }
      translateEscapes = true;
    }
    extracted.append(source.current());
    stringLength++;
    if (stringLength > String::getMaxStringLength()) { JSON_THROW(SyntaxError("String size exceeds maximum allowed size.")); }
    source.next();
  }
  if (source.current() != '"') { JSON_THROW(SyntaxError(source.getPosition(), "Missing closing '\"' on string.")); }
  if (translateEscapes) { extracted = String{translator.from(extracted.value())}; }
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
  return source.isWS() || source.current() == JSON_Lib::kComma || source.current() == JSON_Lib::kArrayEnd
         || source.current() == JSON_Lib::kObjectEnd;
}
/// <summary>
/// Parse an Object key/value pair from a JSON encoded source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Object key/value pair.</returns>
Object::Entry
  Default_Parser::parseObjectEntry(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  source.ignoreWS();
  std::string key{ extractString(source, translator).value() };
  source.ignoreWS();
  if (source.current() != JSON_Lib::kColon) {
    JSON_THROW(SyntaxError(source.getPosition(), "Missing ':' in key value pair."));
  }
  source.next();
  return { std::move(key), parseNodes(source, translator, parserDepth + 1) };
}
/// <summary>
/// Parse a string from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>String Node.</returns>
Node Default_Parser::parseString(ISource &source,
  const ITranslator &translator,
  unsigned long)
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
Node Default_Parser::parseNumber(ISource &source,
  const ITranslator &,
  unsigned long)
{
  static constexpr std::size_t kMaxNumberLength = 64;
  std::array<char, kMaxNumberLength> numberText{};
  std::size_t numberLength = 0;
  while (source.more() && !endOfNumber(source)) {
    if (numberLength >= numberText.size()) { JSON_THROW(SyntaxError("Number size exceeds maximum allowed length.")); }
    numberText[numberLength++] = source.current();
    source.next();
  }
  const std::string_view numberView(numberText.data(), numberLength);
  Number number{ numberView };
  if (number.isValid()) {
    return Node::make<Number>(number);
  }
  JSON_THROW(SyntaxError(source.getPosition(), "Invalid numeric value."));
}
/// <summary>
/// Parse a boolean from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Boolean Node.</returns>
Node Default_Parser::parseBoolean(ISource &source,
  const ITranslator &,
  unsigned long)
{
  static constexpr std::string_view kTrueToken{"true"};
  static constexpr std::string_view kFalseToken{"false"};
  if (source.match(kTrueToken)) { return Node::make<Boolean>(true); }
  if (source.match(kFalseToken)) { return Node::make<Boolean>(false); }
  JSON_THROW(SyntaxError(source.getPosition(), "Invalid boolean value."));
}
/// <summary>
/// Parse a null from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Null Node.</returns>
Node Default_Parser::parseNull(ISource &source,
  const ITranslator &,
  unsigned long)
{
  static constexpr std::string_view kNullToken{"null"};
  if (!source.match(kNullToken)) { JSON_THROW(SyntaxError(source.getPosition(), "Invalid null value.")); }
  return Node::make<Null>();
}
/// <summary>
/// Parse an object from a JSON source stream.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="translator">String translator.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Object Node (key/value pairs).</returns>
/// Parse a collection (object or array) from a JSON source stream.
/// Handles the open / first-element / comma-loop / check-close / advance pattern
/// common to both containers.
template<typename NodeType, typename ParseFn>
static Node parseCollection(ISource &source,
  const ITranslator &translator,
  const unsigned long parserDepth,
  const char closeChar,
  const char *missingCloseMsg,
  ParseFn parseElement)
{
  Node jNode = Node::make<NodeType>();
  source.next();
  source.ignoreWS();
  if (source.current() != closeChar) {
    NRef<NodeType>(jNode).add(parseElement(source, translator, parserDepth));
    while (source.current() == ',') {
      source.next();
      NRef<NodeType>(jNode).add(parseElement(source, translator, parserDepth));
    }
  }
  if (source.current() != closeChar) { JSON_THROW(SyntaxError(source.getPosition(), missingCloseMsg)); }
  source.next();
  return jNode;
}
Node Default_Parser::parseObject(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  return parseCollection<Object>(
    source, translator, parserDepth,
    JSON_Lib::kObjectEnd, "Missing closing '}' in object definition.",
    [](ISource &src, const ITranslator &tr, unsigned long depth) {
      return parseObjectEntry(src, tr, depth);
    });
}
Node Default_Parser::parseArray(ISource &source, const ITranslator &translator, const unsigned long parserDepth)
{
  return parseCollection<Array>(
    source, translator, parserDepth,
    JSON_Lib::kArrayEnd, "Missing closing ']' in array definition.",
    [](ISource &src, const ITranslator &tr, unsigned long depth) {
      return parseNodes(src, tr, depth + 1);
    });
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
  if (parserDepth >= getMaxParserDepth()) { JSON_THROW(SyntaxError("Maximum parser depth exceeded.")); }
  source.ignoreWS();
  const char nextChar = source.current();
  Node jNode;
  switch (nextChar) {
    case JSON_Lib::kObjectBegin:
      jNode = parseObject(source, translator, parserDepth);
      break;
    case JSON_Lib::kArrayBegin:
      jNode = parseArray(source, translator, parserDepth);
      break;
    case JSON_Lib::kStringQuote:
    case JSON_Lib::kStringSingleQuote:
      jNode = parseString(source, translator, parserDepth);
      break;
    case JSON_Lib::kPlus:
    case JSON_Lib::kMinus:
    case JSON_Lib::kZero:
    case JSON_Lib::kOne:
    case JSON_Lib::kTwo:
    case JSON_Lib::kThree:
    case JSON_Lib::kFour:
    case JSON_Lib::kFive:
    case JSON_Lib::kSix:
    case JSON_Lib::kSeven:
    case JSON_Lib::kEight:
    case JSON_Lib::kNine:
      jNode = parseNumber(source, translator, parserDepth);
      break;
    case 't':
    case 'f':
      jNode = parseBoolean(source, translator, parserDepth);
      break;
    case 'n':
      jNode = parseNull(source, translator, parserDepth);
      break;
    default:
      JSON_THROW(SyntaxError(source.getPosition(), "Missing String, Number, Boolean, Array, Object or Null."));
  }
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
Result<Node> Default_Parser::parseResult(ISource &source)
{
#if JSON_LIB_NO_EXCEPTIONS
  // Under NO_EXCEPTIONS, sources that fail construction (e.g. FixedBufferSource{nullptr,0})
  // set an invalid state rather than throwing. Detect this before parseNodes can abort.
  if (!source.more()) {
    return {Status::InvalidInput, nullptr, "Empty or invalid source buffer.", source.getPosition()};
  }
#endif
  try {
    return {Status::Ok, std::make_unique<Node>(parseNodes(source, jsonTranslator, 1)), {}, {0, 0}};
  } catch (const SyntaxError &ex) {
    return {Status::SyntaxError, nullptr, ex.what(), source.getPosition()};
  } catch (const Error &ex) {
    return {Status::UnknownError, nullptr, ex.what(), source.getPosition()};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, nullptr, ex.what(), source.getPosition()};
  }
}
}// namespace JSON_Lib
