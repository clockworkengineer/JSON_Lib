#pragma once
// ===================
// Converter interface
// ===================
#include "IConverter.hpp"
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// ================
// CLASS DEFINITION
// ================
class JSON_Converter : public IConverter {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // ==============
  // PUBLIC METHODS
  // ==============
  [[nodiscard]] std::u16string toUtf16(const std::string &utf8) const override;
  [[nodiscard]] std::string toUtf8(const std::u16string &utf16) const override;
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
};
} // namespace JSON_Lib
