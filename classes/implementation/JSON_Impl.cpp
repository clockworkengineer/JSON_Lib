//
// Class: JSON_Impl
//
// Description: High level entry points in JSON class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Impl.hpp"

namespace JSON_Lib {

void JSON_Impl::intializeConverter()
{
  if (jsonConverter == nullptr) { jsonConverter = std::make_unique<JSON_Converter>(); }
}

void JSON_Impl::intializeTranslator()
{
  if (jsonTranslator == nullptr) { jsonTranslator = std::make_unique<JSON_Translator>(*jsonConverter); }
}

std::string JSON_Impl::version() const
{
  std::stringstream versionString;
  versionString << "JSON_Lib Version  " << JSON_VERSION_MAJOR << "." << JSON_VERSION_MINOR << "." << JSON_VERSION_PATCH;
  return (versionString.str());
}

void JSON_Impl::translator(ITranslator *translator)
{
  if (translator == nullptr) {
    intializeTranslator();
  } else {
    jsonTranslator.reset(translator);
  }
}

void JSON_Impl::converter(IConverter *converter)
{
  if (converter == nullptr) {
    intializeConverter();
  } else {
    jsonConverter.reset(converter);
  }
}

void JSON_Impl::parse(ISource &source) { jNodeRoot = parseJNodes(source); }

void JSON_Impl::stringify(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to stringify."); }
  stringifyJNodes(jNodeRoot, destination, 0);
}

void JSON_Impl::print(IDestination &destination) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to print."); }
  stringifyJNodes(jNodeRoot, destination, printIndent);
}

void JSON_Impl::strip(ISource &source, IDestination &destination) const
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
  traverseJNodes(jNodeRoot, action);
}
void JSON_Impl::traverse(IAction &action) const
{
  if (jNodeRoot.isEmpty()) { throw Error("No JSON to traverse."); }
  traverseJNodes(jNodeRoot, action);
}

JNode &JSON_Impl::operator[](const std::string &key)
{
  try {
    if (jNodeRoot.isEmpty()) { jNodeRoot = JNode::make<Object>(); }
    return (jNodeRoot[key]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Object>(jNodeRoot).add(Object::Entry(key, JNode::make<Hole>()));
    return (jNodeRoot[key]);
  }
}
const JNode &JSON_Impl::operator[](const std::string &key) const { return ((jNodeRoot)[key]); }

JNode &JSON_Impl::operator[](std::size_t index)
{
  try {
    if (jNodeRoot.isEmpty()) { jNodeRoot = JNode::make<Array>(); }
    return (jNodeRoot[index]);
  } catch ([[maybe_unused]] JNode::Error &error) {
    JRef<Array>(jNodeRoot).resize(index);
    return (jNodeRoot[index]);
  }
}
const JNode &JSON_Impl::operator[](std::size_t index) const { return ((jNodeRoot)[index]); }

}// namespace JSON_Lib
