#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class JSON_Impl
{
public:
  // Constructors/Destructors
  JSON_Impl(IStringify *stringify, IParser *parser);
  JSON_Impl(const JSON_Impl &other) = delete;
  JSON_Impl &operator=(const JSON_Impl &other) = delete;
  JSON_Impl(JSON_Impl &&other) = delete;
  JSON_Impl &operator=(JSON_Impl &&other) = delete;
  ~JSON_Impl() = default;
  // Get JSON_Lib version
  static std::string version();
  // Parse JSON into Node tree
  void parse(ISource &source);
  // Create JSON text string (no white space) from Node tree
  void stringify(IDestination &destination) const;
  // Create JSON structured text string (pretty print) from Node tree
  void print(IDestination &destination) const;
  // Strip whitespace from JSON string
  static void strip(ISource &source, IDestination &destination);
  // Get the root of JSON tree
  [[nodiscard]] Node &root() { return jNodeRoot; }
  [[nodiscard]] const Node &root() const { return jNodeRoot; }
  // Search for JSON object entry with a given key
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  // Get JSON array element at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Set print ident value
  static void setIndent(const long indent) { jsonStringify->setIndent(indent); }
  // Read/Write JSON from a file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &jsonString, JSON::Format format);
  // Get JSON file format
  static JSON::Format getFileFormat(const std::string_view &fileName);

private:
  // Traverse JSON tree
  template<typename T> static void traverseNodes(T &jNode, IAction &action);
  // Root of JSON tree
  Node jNodeRoot;
  // Pointer to JSON parser interface
  inline static std::unique_ptr<IParser> jsonParser;
  // Pointer to JSON stringify interface
  inline static std::unique_ptr<IStringify> jsonStringify;
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> jsonTranslator;
};
/// <summary>
/// Recursively traverse Node tree calling IAction methods and possibly
/// modifying the tree contents or even structure.
/// </summary>
/// <param name="jNode">Node tree to be traversed.</param>
/// <param name="action">Action methods to call during traversal.</param>
template<typename T> void JSON_Impl::traverseNodes(T &jNode, IAction &action)
{
  action.onNode(jNode);
  if (isA<Number>(jNode)) {
    action.onNumber(jNode);
  } else if (isA<String>(jNode)) {
    action.onString(jNode);
  } else if (isA<Boolean>(jNode)) {
    action.onBoolean(jNode);
  } else if (isA<Null>(jNode)) {
    action.onNull(jNode);
  } else if (isA<Object>(jNode)) {
    action.onObject(jNode);
    for (auto &entry : JRef<Object>(jNode).value()) { traverseNodes(entry.getNode(), action); }
  } else if (isA<Array>(jNode)) {
    action.onArray(jNode);
    for (auto &entry : JRef<Array>(jNode).value()) { traverseNodes(entry, action); }
  } else if (!isA<Hole>(jNode)) {
    throw Error("Unknown Node type encountered during tree traversal.");
  }
}
}// namespace JSON_Lib
