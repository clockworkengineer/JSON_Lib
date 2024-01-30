#pragma once

#include <string>
#include <stdexcept>

namespace JSON_Lib {

// ========================
// Unicode surrogate ranges
// ========================

constexpr char16_t kHighSurrogatesBegin{ 0xD800 };
constexpr char16_t kHighSurrogatesEnd{ 0xDBFF };
constexpr char16_t kLowSurrogatesBegin{ 0xDC00 };
constexpr char16_t kLowSurrogatesEnd{ 0xDFFF };

// =========================================================
// Interface for translation to/from JSON escaped characters
// =========================================================
class ITranslator
{
public:
  // =============
  // ITranslator Error
  // =============
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("ITranslator Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~ITranslator() = default;
  // =====================================================================
  // Convert any escape sequences in a string to their correct sequence
  // of UTF-8 characters. If input string contains any unpaired surrogates
  // then this is deemed as a syntax error and an error is duly thrown.
  // =====================================================================
  virtual std::string from(const std::string &jsonString) = 0;
  // =========================================================================
  // Convert a string from raw charater values (UTF8) so that it has character
  // escapes where applicable for its JSON form.
  // =========================================================================
  virtual std::string to(const std::string &utf8String) = 0;
  // =======================================================
  // Return true if a character is a valid escaped character
  // =======================================================
  virtual bool validEscape(char escape) = 0;
};
}// namespace JSON_Lib