#pragma once

#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "JSON_Config.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class JSON_Impl
{
public:
  JSON_Impl() = default;
  JSON_Impl(const JSON_Impl &other) = delete;
  JSON_Impl &operator=(const JSON_Impl &other) = delete;
  JSON_Impl(JSON_Impl &&other) = delete;
  JSON_Impl &operator=(JSON_Impl &&other) = delete;
  ~JSON_Impl() = default;

  // Get JSON_Lib version
  std::string version() const;
  // Parse JSON into tree
  void parse(ISource &source);
  // Create JSON text string (no white space) from JNode tree
  void stringify(IDestination &destination) const;
  // Create JSON structured text string (pretty print) from JNode tree
  void print(IDestination &destination) const;
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination) const;
  // Set JSON translator/converter
  static void setTranslator(ITranslator *translator);
  static void setConverter(IConverter *converter);
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
  void setIndent(long indent)
  {
    if (indent < 0) { throw JSON::Error("Invalid print indentation value."); };
    printIndent = indent;
  }
  // Read/Write JSON from file
   static const std::string fromFile(const std::string &fileName);
   static void toFile(const std::string &fileName, const std::string &jsonString, JSON::Format format);
  // Get JSON file format
   static JSON::Format getFileFormat(const std::string &fileName);

private:
  // Initialize converter/translator
   static void intializeConverter();
   static void intializeTranslator();
  // Stringify JSON
   void stringifyNumber(const JNode &jNode, IDestination &destination)const ;
   void stringifyString(const JNode &jNode, IDestination &destination)const;
   void stringifyBoolean(const JNode &jNode, IDestination &destination)const;
   void stringifyNull(const JNode &jNode, IDestination &destination)const ;
   void stringifyHole(const JNode &jNode, IDestination &destination)const;
   void stringifyObject(const JNode &jNode, IDestination &destination, long indent)const;
   void stringifyArray(const JNode &jNode, IDestination &destination, long indent)const;
  // Produce JSON test string from JSON tree
   void stringifyJNodes(const JNode &jNode, IDestination &destination, long indent)const;
  // Traverse JSON tree
  template<typename T> static void traverseJNodes(T &jNode, IAction &action);
  // Root of JSON tree
  JNode jNodeRoot;
  inline static std::unique_ptr<IParser> jsonParser;
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> jsonTranslator;
  // Pointer to character conversion interface
  inline static std::unique_ptr<IConverter> jsonConverter;
  // Current print indent value
  inline static long printIndent{ 4 }; 
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
    for (auto &entry : JRef<Object>(jNode).getObjectEntries()) { traverseJNodes(entry.getJNode(), action); }
    break;
  case Variant::Type::array:
    action.onArray(jNode);
    for (auto &entry : JRef<Array>(jNode).getArrayEntries()) { traverseJNodes(entry, action); }
    break;
  default:
    throw JSON::Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSON_Lib
