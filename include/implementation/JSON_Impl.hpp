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
  std::string version();
  JNode::Ptr parse(ISource &source);
  JNode::Ptr parse(const std::string &jsonString);
  void stringify(const JNode &jNodeRoot, IDestination &destination);
  void strip(ISource &source, IDestination &destination);
  void translator(ITranslator *translator);
  void converter(IConverter *converter);
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
  static std::string extractString(ISource &source, bool translate = true);
  static JNodeObject::ObjectEntry parseKeyValuePair(ISource &source);
  static JNode::Ptr parseString(ISource &source);
  static JNode::Ptr parseNumber(ISource &source);
  static JNode::Ptr parseBoolean(ISource &source);
  static JNode::Ptr parseNull(ISource &source);
  static JNode::Ptr parseObject(ISource &source);
  static JNode::Ptr parseArray(ISource &source);
  static JNode::Ptr parseJNodes(ISource &source);
  static void stringifyJNodes(const JNode &jNode, IDestination &destination);
  static void stripWhiteSpace(ISource &source, IDestination &destination);
  // =================
  // PRIVATE VARIABLES
  // =================
  // Pointer to JSON translator interface
  inline static std::unique_ptr<ITranslator> m_translator;
  // Pointer to character conversion interface
  inline static std::unique_ptr<IConverter> m_converter;
};
} // namespace JSONLib
