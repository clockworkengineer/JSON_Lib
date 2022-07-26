#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ===================================================
// Forward declarations for interfaces/classes/structs
// ===================================================
class JSON_Impl;
class ISource;
class IDestination;
class IConverter;
class ITranslator;
struct JNode;
// ================
// CLASS DEFINITION
// ================
class JSON {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // Pass any user defined translator/converter here
  explicit JSON(ITranslator *translator = nullptr,
                IConverter *converter = nullptr);
  // Pass in default JSON to parse
  explicit JSON(const std::string &jsonString);
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
  // Create JSON text string from tree
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination) const;
  void strip(ISource &source, IDestination &&destination) const;
  void strip(ISource &&source, IDestination &destination) const;
  void strip(ISource &&source, IDestination &&destination) const;
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
  const std::unique_ptr<JSON_Impl> m_jsonImplementation;
};
} // namespace JSONLib