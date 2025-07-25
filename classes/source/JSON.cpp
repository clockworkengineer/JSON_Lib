//
// Class: JSON
//
// Description: Perform JSON parse/stringify to/from a buffer or
// file. For an in-depth description of the JSON specification refer
// to its RFC at https://tools.ietf.org/html/rfc8259 and the JSON grammar
// at https://www.json.org/json-en.html.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON_Impl.hpp"

namespace JSON_Lib {

/// <summary>
/// JSON constructor. Set any custom converter or translator here.
/// </summary>
/// <param name="stringify">Pointer to stringifier interface.</param>
/// <param name="parser">Pointer to parser interface.</param>
JSON::JSON(IStringify *stringify, IParser *parser) : implementation(std::make_unique<JSON_Impl>(stringify, parser))
{
}
/// <summary>
/// JSON constructor (array).
/// </summary>
/// <param name="array">Initializer list of single values or Node.</param>
JSON::JSON(const ArrayInitializer &array) : JSON() { this->root() = Node(array); }
/// <summary>
/// JSON constructor (object).
/// </summary>
/// <param name="object">Initializer list of key/value(Node) pairs.</param>
JSON::JSON(const ObjectInitializer &object) : JSON() { this->root() = Node(object); }
/// <summary>
/// JSON constructor. Pass a JSON string to be initially parsed.
/// </summary>
/// <param name="jsonString">JSON string.</param>
JSON::JSON(const std::string_view &jsonString) : JSON() { parse(BufferSource{ jsonString }); }
/// <summary>
/// JSON destructor.
/// </summary>
JSON::~JSON() = default;
/// <summary>
/// Get JSON library version.
/// </summary>
std::string JSON::version() { return JSON_Impl::version(); }
/// <summary>
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON::strip(ISource &source, IDestination &destination) { JSON_Impl::strip(source, destination); }
void JSON::strip(ISource &source, IDestination &&destination)  { JSON_Impl::strip(source, destination); }
void JSON::strip(ISource &&source, IDestination &destination)  { JSON_Impl::strip(source, destination); }
void JSON::strip(ISource &&source, IDestination &&destination)  { JSON_Impl::strip(source, destination); }
/// <summary>
/// Create Node structure by parsing JSON on the source stream.
/// </summary>
/// <param name="source">Source for JSON encoded bytes.</param>
void JSON::parse(ISource &source) const { implementation->parse(source); }
void JSON::parse(ISource &&source) const { implementation->parse(source); }
/// <summary>
/// Traverse Node structure and build its JSON string (no whitespace) on destination stream.
/// </summary>
/// <param name="destination">Destination stream for stringified JSON.</param>
void JSON::stringify(IDestination &destination) const { implementation->stringify(destination); }
void JSON::stringify(IDestination &&destination) const { implementation->stringify(destination); }
/// <summary>
/// Traverse Node structure and build its JSON string (pretty printed) on destination stream.
/// </summary>
/// <param name="destination">Destination stream for stringified JSON.</param>
void JSON::print(IDestination &destination) const { implementation->print(destination); }
void JSON::print(IDestination &&destination) const { implementation->print(destination); }
/// <summary>
/// Set print indent value.
/// </summary>
/// <param name="indent">Pretty print indent value.</param>
void JSON::setIndent(const long indent) { JSON_Impl::setIndent(indent); }
/// <summary>
/// Recursively traverse Node structure calling IAction methods (read-only)
///  or to change the JSON tree node directly.
/// </summary>
/// <param name="action">Action methods to call during traversal.</param>
/// Traverse using non-const JSON so can change JSON tree
void JSON::traverse(IAction &action) { implementation->traverse(action); }
// Traverse using const JSON so cannot change JSON tree
void JSON::traverse(IAction &action) const { std::as_const(*implementation).traverse(action); }
/// <summary>
/// Return object entry for the passed in keys.
/// </summary>
/// <param name="key">Object entry (Node) key.</param>
Node &JSON::operator[](const std::string_view &key) { return (*implementation)[key]; }
const Node &JSON::operator[](const std::string_view &key) const { return (*implementation)[key]; }
/// <summary>
/// Return array entry for the passed in index.
/// </summary>
/// <param name="index">Array entry (Node) index.</param>
Node &JSON::operator[](const std::size_t index) { return (*implementation)[index]; }
const Node &JSON::operator[](const std::size_t index) const { return (*implementation)[index]; }
/// <summary>
/// Return root of JSON tree.
/// </summary>
/// <returns>Root of JSON tree.</returns>
Node &JSON::root() { return implementation->root(); }
const Node &JSON::root() const { return implementation->root(); }
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON string.</returns>
std::string JSON::fromFile(const std::string_view &fileName) { return JSON_Impl::fromFile(fileName); }
/// <summary>
/// Create an JSON file and write JSON string to it.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <param name="jsonString">JSON string</param>
/// <param name="format">JSON file format</param>
void JSON::toFile(const std::string_view &fileName, const std::string_view &jsonString, const Format format)
{
  JSON_Impl::toFile(fileName, jsonString, format);
}
/// <summary>
/// Return format of JSON file.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON file format.</returns>
JSON::Format JSON::getFileFormat(const std::string_view &fileName) { return JSON_Impl::getFileFormat(fileName); }
}// namespace JSON_Lib
