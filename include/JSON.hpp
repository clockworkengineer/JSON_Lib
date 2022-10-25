#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <sstream>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ========================================================
// JSON forward declarations for interfaces/classes/structs
// ========================================================
#include "JSON_forward.hpp"
// ================
// CLASS DEFINITION
// ================
class JSON
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // Possible JSON Node internal value types
  using InternalType =
    std::variant<int, long, long long, float, double, long double, bool, std::string, std::nullptr_t, JNode>;
  // Array initializer list
  using ArrayList = std::initializer_list<InternalType>;
  // Object initializer list
  using ObjectList = std::initializer_list<std::pair<std::string, InternalType>>;
  // JSON file formats
  enum class Format : uint8_t { utf8=0, utf8BOM, utf8BE, utf8LE, utf16BE, utf16LE, utf32BE, utf32LE };
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // Pass any user defined translator/converter here
  explicit JSON(ITranslator *translator = nullptr, IConverter *converter = nullptr);
  // Pass in default JSON to parse
  explicit JSON(const std::string &jsonString);
  // Construct array
  JSON(const ArrayList &arrayList);
  // Construct object
  JSON(const ObjectList &objectList);
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
  // Get JSON library version
  [[nodiscard]] std::string version() const;
  // Parse JSON into tree
  void parse(ISource &source) const;
  void parse(ISource &&source) const;
  // Create JSON text string from JNode tree (no shitespace)
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  // Create JSON text string from JNode tree (pretty printed)
  void print(IDestination &destination) const;
  void print(IDestination &&destination) const;
  // Strip whitespace from JSON string
  void strip(ISource &source, IDestination &destination) const;
  void strip(ISource &source, IDestination &&destination) const;
  void strip(ISource &&source, IDestination &destination) const;
  void strip(ISource &&source, IDestination &&destination) const;
  // Traverse JSON tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Set print ident value
  void setIndent(long indent) const;
  // Get root of JSON tree
  [[nodiscard]] JNode &root();
  [[nodiscard]] const JNode &root() const;
  // Search for JSON object entry with a given key
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  // Get JSON array entry at index
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Read/Write JSON from file
  static const std::string fromFile(const std::string &jsonFileName);
  static void toFile(const std::string &jsonFileName, const std::string &jsonString);
  // Get JSON file format
  static Format getFileFormat(const std::string &jsonFileName);
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
}// namespace JSON_Lib