//
// Class: JSON_Impl
//
// Description: High level entry points in JSON class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON_Impl.hpp"
#include "JSON_Throw.hpp"
#include <string>

namespace JSON_Lib {

JSON_Impl::JSON_Impl(std::unique_ptr<IStringify> stringify, std::unique_ptr<IParser> parser)
{
  jsonTranslator = std::make_unique<Default_Translator>();
  if (parser == nullptr) {
    jsonParser = std::make_unique<Default_Parser>(*jsonTranslator);
  } else {
    jsonParser = std::move(parser);
  }
  if (stringify == nullptr) {
    jsonStringify = std::make_unique<Default_Stringify>();
  } else {
    jsonStringify = std::move(stringify);
  }
}
void JSON_Impl::setIndent(const long indent)
{
  jsonStringify->setIndent(indent);
}
void JSON_Impl::setMaxParserDepth(const unsigned long depth) { jsonParser->setMaxParserDepth(depth); }
unsigned long JSON_Impl::getMaxParserDepth() const noexcept { return jsonParser->getMaxParserDepth(); }
std::string JSON_Impl::version()
{
  std::string v;
  v.reserve(32);
  v.append("JSON_Lib Version  ")
   .append(std::to_string(JSON_VERSION_MAJOR)).append(".")
   .append(std::to_string(JSON_VERSION_MINOR)).append(".")
   .append(std::to_string(JSON_VERSION_PATCH));
  return v;
}
void JSON_Impl::parse(ISource &source) { jNodeRoot = jsonParser->parse(source); }
Result<Node> JSON_Impl::parseResult(ISource &source)
{
  auto result = jsonParser->parseResult(source);
  if (result.ok() && result.value) { jNodeRoot = std::move(*result.value); }
  return result;
}
void JSON_Impl::stringify(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { JSON_THROW(Error("No JSON to stringify.")); }
  jsonStringify->stringify(jNodeRoot, destination, 0);
}
Result<void> JSON_Impl::runStringify(IDestination &destination, unsigned long indent) const
{
  if (jNodeRoot.isEmpty()) {
    return {Status::InvalidInput, {}, {0, 0}};
  }
  try {
    jsonStringify->stringify(jNodeRoot, destination, indent);
    return {Status::Ok, {}, {0, 0}};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, ex.what(), {0, 0}};
  }
}
Result<void> JSON_Impl::stringifyResult(IDestination &destination) const
{
  return runStringify(destination, 0);
}
void JSON_Impl::print(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { JSON_THROW(Error("No JSON to print.")); }
  jsonStringify->stringify(jNodeRoot, destination, jsonStringify->getIndent());
}
Result<void> JSON_Impl::printResult(IDestination &destination) const
{
  return runStringify(destination, jsonStringify->getIndent());
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
  if (jNodeRoot.isEmpty()) { JSON_THROW(Error("No JSON to traverse.")); }
  traverseNodes(jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (jNodeRoot.isEmpty()) { JSON_THROW(Error("No JSON to traverse.")); }
  traverseNodes(jNodeRoot, action);
}
Result<void> JSON_Impl::runTraverse(IAction &action)
{
  if (jNodeRoot.isEmpty()) {
    return {Status::InvalidInput, {}, {0, 0}};
  }
  try {
    traverseNodes(jNodeRoot, action);
    return {Status::Ok, {}, {0, 0}};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, ex.what(), {0, 0}};
  }
}
Result<void> JSON_Impl::runTraverse(IAction &action) const
{
  if (jNodeRoot.isEmpty()) {
    return {Status::InvalidInput, {}, {0, 0}};
  }
  try {
    traverseNodes(jNodeRoot, action);
    return {Status::Ok, {}, {0, 0}};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, ex.what(), {0, 0}};
  }
}
Node &JSON_Impl::operator[](const std::string_view &key)
{
  try {
    if (jNodeRoot.isEmpty()) { jNodeRoot = Node::make<Object>(); }
    return jNodeRoot[key];
  } catch (Node::Error &) {
    NRef<Object>(jNodeRoot).add(Object::Entry(key, Node::make<Hole>()));
    return jNodeRoot[key];
  }
}
const Node &JSON_Impl::operator[](const std::string_view &key) const { return jNodeRoot[key]; }
Node &JSON_Impl::operator[](const std::size_t index)
{
  if (jNodeRoot.isEmpty()) {
    jNodeRoot = Node::make<Array>();
    NRef<Array>(jNodeRoot).resize(index);
    return jNodeRoot[index];
  }
  return jNodeRoot[index];
}
const Node &JSON_Impl::operator[](const std::size_t index) const { return jNodeRoot[index]; }

void JSON_Impl::resize(std::size_t index)
{
  if (jNodeRoot.isEmpty()) { jNodeRoot = Node::make<Array>(); }
  jNodeRoot.resize(index);
}
}// namespace JSON_Lib
