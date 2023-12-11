#pragma once

#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Parser.hpp"
#include "JSON_Stringify.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"

namespace JSON_Lib {

class JSON_Impl
{
public:
  // Constructors/Destructors
  JSON_Impl() = default;
  JSON_Impl(const JSON_Impl &other) = delete;
  JSON_Impl &operator=(const JSON_Impl &other) = delete;
  JSON_Impl(JSON_Impl &&other) = delete;
  JSON_Impl &operator=(JSON_Impl &&other) = delete;
  ~JSON_Impl() = default;
  // Get JSON_Lib version
  std::string version() const;
  // Parse JSON into JNode tree
  void parse(ISource &source);
  // Create JSON text string (no white space) from JNode tree
  void stringify(IDestination &destination) const;
  // Create JSON structured text string (pretty print) from JNode tree
  void print(IDestination &destination) const;
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination) const;
  // Set JSON Parser/Stringify
  static void setParser(IParser *parser = nullptr);
  static void setStringify(IStringify *stringify = nullptr);
  // Set JSON translator/converter
  static void setTranslator(ITranslator *translator = nullptr);
  static void setConverter(IConverter *converter = nullptr);
  // Get root of JSON tree
  [[nodiscard]] JNode &root() { return (jNodeRoot); }
  [[nodiscard]] const JNode &root() const { return (jNodeRoot); }
  // Search for JSON object entry with a given key
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  // Get JSON array element at index
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Set print ident value
  void setIndent(long indent) { jsonStringify->setIndent(indent); }
  // Read/Write JSON from file
  static const std::string fromFile(const std::string &fileName);
  static void toFile(const std::string &fileName, const std::string &jsonString, JSON::Format format);
  // Get JSON file format
  static JSON::Format getFileFormat(const std::string &fileName);

private:
  // Traverse JSON tree
  template<typename T> static void traverseJNodes(T &jNode, IAction &action);
  // Root of JSON tree
  JNode jNodeRoot;
  // Pointer to JSON parser interface
  inline static std::unique_ptr<IParser> jsonParser;
  // Pointer to JSON stringify interface
  inline static std::unique_ptr<IStringify> jsonStringify;
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> jsonTranslator;
  // Pointer to character conversion interface
  inline static std::unique_ptr<IConverter> jsonConverter;
};
/// <summary>
/// Recursively traverse JNode tree calling IAction methods and possibly
/// modifying the tree contents or even structure.
/// </summary>
/// <param name=jNode>JNode tree to be traversed.</param>
/// <param name=action>Action methods to call during traversal.</param>
template<typename T> void JSON_Impl::traverseJNodes(T &jNode, IAction &action)
{
  action.onJNode(jNode);
  switch (jNode.getVariant().getType()) {
  case Variant::Type::number:
    action.onNumber(jNode);
    break;
  case Variant::Type::string:
    action.onString(jNode);
    break;
  case Variant::Type::boolean:
    action.onBoolean(jNode);
    break;
  case Variant::Type::null:
    action.onNull(jNode);
    break;
  case Variant::Type::hole:
    break;
  case Variant::Type::object:
    action.onObject(jNode);
    for (auto &entry : JRef<Object>(jNode).value()) { traverseJNodes(entry.getJNode(), action); }
    break;
  case Variant::Type::array:
    action.onArray(jNode);
    for (auto &entry : JRef<Array>(jNode).value()) { traverseJNodes(entry, action); }
    break;
  default:
    throw JSON::Error("Unknown JNode type encountered during tree traversal.");
  }
}
}// namespace JSON_Lib
