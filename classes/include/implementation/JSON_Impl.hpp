#pragma once
#include "JSON_Throw.hpp"

#include <memory>
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
  Result<Node> parseResult(ISource &source);
  // Create JSON text string (no white space) from Node tree
  void stringify(IDestination &destination) const;
  Result<void> stringifyResult(IDestination &destination) const;
  // Create JSON structured text string (pretty print) from Node tree
  void print(IDestination &destination) const;
  Result<void> printResult(IDestination &destination) const;
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
  static void setIndent(const long indent);
#if !JSON_LIB_NO_STDIO
  // Read/Write JSON from a file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &jsonString, JSON::Format format);
  // Get JSON file format
  static JSON::Format getFileFormat(const std::string_view &fileName);
#endif

private:
  // Helper: empty-guard + try/catch wrapper for stringify operations
  Result<void> runStringify(IDestination &destination, unsigned long indent) const;
  // Traverse JSON tree
  template<typename T> static void traverseNodes(T &jNode, IAction &action);
  // Root of JSON tree
  Node jNodeRoot;
  // Parser/stringifier/translator instances for this JSON object.
  std::unique_ptr<IParser> jsonParser;
  std::unique_ptr<IStringify> jsonStringify;
  std::unique_ptr<ITranslator> jsonTranslator;
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
  jNode.visit(overloaded{
    [&](const Number &) { action.onNumber(jNode); },
    [&](const String &) { action.onString(jNode); },
    [&](const Boolean &) { action.onBoolean(jNode); },
    [&](const Null &) { action.onNull(jNode); },
    [&](const Object &) {
      action.onObject(jNode);
      for (auto &entry : NRef<Object>(jNode).value()) { traverseNodes(entry.getNode(), action); }
    },
    [&](const Array &) {
      action.onArray(jNode);
      for (auto &entry : NRef<Array>(jNode).value()) { traverseNodes(entry, action); }
    },
    [&](const Hole &) {},
    [&](const std::monostate &) { JSON_THROW(Error("Unknown Node type encountered during tree traversal.")); }
  });
}
}// namespace JSON_Lib
