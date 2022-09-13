#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
// =============================
// Source/Destination interfaces
// =============================
#include "ISource.hpp"
#include "IDestination.hpp"
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
#include "JSON_Translator.hpp"
#include "JSON_Types.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
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
  // Get JSONLib version
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
  // Has end of number been reached ?
  static bool endOfNumber(ISource &source);
  // Parse JSON
  static std::string extractString(ISource &source, bool translate = true);
  static Object::Entry parseKeyValuePair(ISource &source);
  static JNode parseString(ISource &source);
  static JNode parseNumber(ISource &source);
  static JNode parseBoolean(ISource &source);
  static JNode parseNull(ISource &source);
  static JNode parseObject(ISource &source);
  static JNode parseArray(ISource &source);
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
  // Remove JSON whitespace
  static void stripWhitespace(ISource &source, IDestination &destination);
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
  case JNode::Type::number:
    action.onNumber(JRef<Number>(jNode));
    break;
  case JNode::Type::string:
    action.onString(JRef<String>(jNode));
    break;
  case JNode::Type::boolean:
    action.onBoolean(JRef<Boolean>(jNode));
    break;
  case JNode::Type::null:
    action.onNull(JRef<Null>(jNode));
    break;
  case JNode::Type::hole:
    break;
  case JNode::Type::object:
    action.onObject(JRef<Object>(jNode));
    for (auto &entry : JRef<Object>(jNode).getObjectEntries()) { traverseJNodes(entry.getJNode(), action); }
    break;
  case JNode::Type::array:
    action.onArray(JRef<Array>(jNode));
    for (auto &node : JRef<Array>(jNode).getArrayEntries()) { traverseJNodes(node, action); }
    break;
  default:
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSONLib
