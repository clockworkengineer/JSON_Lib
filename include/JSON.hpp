#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ===================================================
// Forward declarations for interfaces/classes/structs
// ===================================================
class JSON_Impl;
struct JNode;
class ISource;
class IDestination;
class IAction;
class IConverter;
class ITranslator;
// ================
// CLASS DEFINITION
// ================
class JSON
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // Possible JSON node value types
  using InternalTypes = std::variant<int, long, float, double, bool, std::string, std::nullptr_t, JNode>;
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // Pass any user defined translator/converter here
  explicit JSON(ITranslator *translator = nullptr, IConverter *converter = nullptr);
  // Pass in default JSON to parse
  explicit JSON(const std::string &jsonString);
  // Construct array
  JSON(const std::initializer_list<InternalTypes> &array);
  // Construct object
  JSON(const std::initializer_list<std::pair<std::string, InternalTypes>> &object);
  // No other constructors supported
  JSON(const JSON &other) = delete;
  JSON &operator=(const JSON &other) = delete;
  JSON(JSON &&other) = delete;
  JSON &operator=(JSON &&other) = delete;
  // Provide own destructor
  ~JSON();
  // ==============
  // PUBLIC METHODS
  // ==============
  // Get JSONLib version
  [[nodiscard]] std::string version() const;
  // Parse JSON into tree
  void parse(ISource &source) const;
  void parse(ISource &&source) const;
  // Create JSON text string from tree (no shitespace)
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination) const;
  void strip(ISource &source, IDestination &&destination) const;
  void strip(ISource &&source, IDestination &destination) const;
  void strip(ISource &&source, IDestination &&destination) const;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Get root of JSON tree
  [[nodiscard]] JNode &root();
  [[nodiscard]] const JNode &root() const;
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
  // =================
  // PRIVATE VARIABLES
  // =================
  // JSON implementation
  const std::unique_ptr<JSON_Impl> m_implementation;
};
}// namespace JSONLib