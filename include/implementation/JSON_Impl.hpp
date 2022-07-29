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
#include "IDestination.hpp"
#include "ISource.hpp"
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
class JSON_Impl {
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
  std::string version();
  // Parse JSON into tree
  void parse(ISource &source);
  void parse(const std::string &jsonString);
  // Create JSON text string from tree
  void stringify(IDestination &destination);
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination);
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
  // Produce JSON test string from JSON tree
  static void stringifyJNodes(const JNode &jNode, IDestination &destination);
  // Remove JSON whitespace
  static void stripWhiteSpace(ISource &source, IDestination &destination);
  // =================
  // PRIVATE VARIABLES
  // =================
  // Root of JSON tree
  JNode m_jNodeRoot;
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> m_translator;
  // Pointer to character conversion interface
  inline static std::unique_ptr<IConverter> m_converter;
};
} // namespace JSONLib
