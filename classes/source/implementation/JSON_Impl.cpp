//
// Class: JSON_Impl
//
// Description: High level entry points in JSON class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON_Impl.hpp"

namespace JSON_Lib {

JSON_Impl::JSON_Impl(IStringify *stringify, IParser *parser)
{
  jsonTranslator = std::make_unique<Default_Translator>();
  if (parser == nullptr) {
    jsonParser = std::make_unique<Default_Parser>(*jsonTranslator);
  } else {
    jsonParser.reset(parser);
  }
  if (stringify == nullptr) {
    jsonStringify = std::make_unique<Default_Stringify>();
  } else {
    jsonStringify.reset(stringify);
  }
}
std::string JSON_Impl::version()
{
  std::stringstream versionString;
  versionString << "JSON_Lib Version  " << JSON_VERSION_MAJOR << "." << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
  return versionString.str();
}
void JSON_Impl::parse(ISource &source) { jNodeRoot = jsonParser->parse(source); }
void JSON_Impl::stringify(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to stringify."); }
  jsonStringify->stringify(jNodeRoot, destination, 0);
}
void JSON_Impl::print(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to print."); }
  jsonStringify->stringify(jNodeRoot, destination, jsonStringify->getIndent());
}
void JSON_Impl::strip(ISource &source, IDestination &destination)
{// Note: That it is assumed that the JSON on the source stream is valid with no errors.
  while (source.more()) {
    if (!source.isWS()) {
      destination.add(source.current());
      if (source.current() == '"') {
        source.next();
        while (source.more() && source.current() != '"') {
          if (source.current() == '\\') {
            destination.add(source.current());
            source.next();
          }
          destination.add(source.current());
          source.next();
        }
        destination.add(source.current());
      }
    }
    source.next();
  }
}
void JSON_Impl::traverse(IAction &action)
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseNodes(jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseNodes(jNodeRoot, action);
}
Node &JSON_Impl::operator[](const std::string_view &key)
{
  try {
    if (jNodeRoot.isEmpty()) { jNodeRoot = Node::make<Object>(); }
    return jNodeRoot[key];
  } catch ([[maybe_unused]] Node::Error &error) {
    NRef<Object>(jNodeRoot).add(Object::Entry(key, Node::make<Hole>()));
    return jNodeRoot[key];
  }
}
const Node &JSON_Impl::operator[](const std::string_view &key) const { return jNodeRoot[key]; }
Node &JSON_Impl::operator[](const std::size_t index)
{
  try {
    if (jNodeRoot.isEmpty()) { jNodeRoot = Node::make<Array>(); }
    return jNodeRoot[index];
  } catch ([[maybe_unused]] Node::Error &error) {
    NRef<Array>(jNodeRoot).resize(index);
    return jNodeRoot[index];
  }
}
const Node &JSON_Impl::operator[](const std::size_t index) const { return jNodeRoot[index]; }
}// namespace JSON_Lib
