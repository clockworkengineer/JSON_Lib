

#pragma once

#include "ITranslator.hpp"

namespace JSON_Lib {

class YAML_Translator final : public ITranslator
{
public:
  // ==============
  // Translator Error
  // ==============
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("Translator Error: " + message) {}
  };
  // ===============================================
  // Translate string to escape sequences ("\uxxxx")
  // ===============================================
  [[nodiscard]] std::string from(const std::string &escapedString) const override { return escapedString; }
  [[nodiscard]] std::string to(const std::string &escapedString) const override
  {
    std::string translated;
    for (const auto ch : toUtf16(escapedString)) {
      if (isASCII(ch) && isprint(ch)) {
        translated += ch;
      } else {
        translated += "\\u";
        const auto digits = "0123456789ABCDEF";
        translated += digits[ch >> 12 & 0x0f];
        translated += digits[ch >> 8 & 0x0f];
        translated += digits[ch >> 4 & 0x0f];
        translated += digits[ch&0x0f];
      }
    }
    return translated;
  }

private:
  /// <summary>
  /// Determine whether passed in character is valid ASCII
  /// </summary>
  /// <param name="utf16Char">UTF16 character.</param>
  /// <returns>True if valid ASCII.</returns>
  [[nodiscard]] static bool isASCII(const char16_t utf16Char) { return utf16Char > 0x001F && utf16Char < 0x0080; }
};
}// namespace JSON_Lib
