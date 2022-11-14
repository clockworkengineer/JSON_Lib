#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
// ==========================
// Traversal action interface
// ==========================
#include "IAction.hpp"
// ====
// JSON
// ====
#include "JSON_Config.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Core.hpp"
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ================
// CLASS DEFINITION
// ================
class JSON_Impl
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  JSON_Impl() = default;
  JSON_Impl(const JSON_Impl &other) = delete;
  JSON_Impl &operator=(const JSON_Impl &other) = delete;
  JSON_Impl(JSON_Impl &&other) = delete;
  JSON_Impl &operator=(JSON_Impl &&other) = delete;
  ~JSON_Impl() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
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
  void translator(ITranslator *translator);
  void converter(IConverter *converter);
  // Get root of JSON tree
  [[nodiscard]] JNode &root() { return (m_jNodeRoot); }
  [[nodiscard]] const JNode &root() const { return (m_jNodeRoot); }
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
    if (indent < 0) { throw Error("Invalid print indentation value."); };
    m_indent = indent;
  }
  // Read/Write JSON from file
  static const std::string fromFile(const std::string &fileName);
  static void toFile(const std::string &fileName, const std::string &jsonString, JSON::Format format);
  // Get JSON file format
  static JSON::Format getFileFormat(const std::string &fileName);
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  // Initialize converter/translator
  static void intializeConverter();
  static void intializeTranslator();
  // Parse JSON
  static Object::Entry parseObjectEntry(ISource &source);
  static JNode parseString(ISource &source);
  static JNode parseNumber(ISource &source);
  static JNode parseBoolean(ISource &source);
  static JNode parseNull(ISource &source);
  static JNode parseObject(ISource &source);
  static JNode parseArray(ISource &source);
  // Produce JSON tree
  static JNode parseJNodes(ISource &source);
  // Stringify JSON
  static void stringifyNumber(const JNode &jNode, IDestination &destination);
  static void stringifyString(const JNode &jNode, IDestination &destination);
  static void stringifyBoolean(const JNode &jNode, IDestination &destination);
  static void stringifyNull(const JNode &jNode, IDestination &destination);
  static void stringifyHole(const JNode &jNode, IDestination &destination);
  static void stringifyObject(const JNode &jNode, IDestination &destination, long indent);
  static void stringifyArray(const JNode &jNode, IDestination &destination, long indent);
  // Produce JSON test string from JSON tree
  static void stringifyJNodes(const JNode &jNode, IDestination &destination, long indent);
  // Traverse JSON tree
  template<typename T> static void traverseJNodes(T &jNode, IAction &action);
  // =================
  // PRIVATE VARIABLES
  // =================
  // Root of JSON tree
  JNode m_jNodeRoot;
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> m_translator;
  // Pointer to character conversion interface
  inline static std::unique_ptr<IConverter> m_converter;
  // Current print indent value
  inline static long m_indent{ 4 };
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
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSON_Lib
