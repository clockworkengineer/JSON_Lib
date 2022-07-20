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
  explicit JSON(ITranslator *translator = nullptr,
                IConverter *converter = nullptr);
  explicit JSON(const std::string &jsonString);
  JSON(const JSON &other) = delete;
  JSON &operator=(const JSON &other) = delete;
  JSON(JSON &&other) = delete;
  JSON &operator=(JSON &&other) = delete;
  ~JSON();
  // ==============
  // PUBLIC METHODS
  // ==============
  [[nodiscard]] std::string version() const;
  void parse(ISource &source) const;
  void parse(ISource &&source) const;
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  void strip(ISource &source, IDestination &destination) const;
  void strip(ISource &source, IDestination &&destination) const;
  void strip(ISource &&source, IDestination &destination) const;
  void strip(ISource &&source, IDestination &&destination) const;
  [[nodiscard]] JNode &root();
  [[nodiscard]] const JNode &root() const;
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
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