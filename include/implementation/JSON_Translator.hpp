#pragma once
// =======
// C++ STL
// =======
#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
// ===============================
// Translator/Converter interfaces
// ===============================
#include "IConverter.hpp"
#include "ITranslator.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ================
// CLASS DEFINITION
// ================
class JSON_Translator : public ITranslator {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // =====================
  // JSON translator error
  // =====================
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("JSON Translator Error: " + message) {}
  };
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // Pass in user defined converter here
  explicit JSON_Translator(const IConverter &converter);
  // No other constructors supported
  JSON_Translator(const JSON_Translator &other) = delete;
  JSON_Translator &operator=(const JSON_Translator &other) = delete;
  JSON_Translator(JSON_Translator &&other) = delete;
  JSON_Translator &operator=(JSON_Translator &&other) = delete;
  ~JSON_Translator() override = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  // Convert to/from JSON escaped characters
  std::string fromJSON(const std::string &jsonString) override;
  std::string toJSON(const std::string &utf8String) override;
  // Return true if character is a valid escape character
  bool validEscape(char escape) override;
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
  // Character converter
  const IConverter &m_converter;
  // To/From escape sequence lookup maps
  std::unordered_map<char, char16_t> m_fromEscape;
  std::unordered_map<char16_t, char> m_toEscape;
};
} // namespace JSONLib
