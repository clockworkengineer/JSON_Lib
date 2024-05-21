#pragma once

#include "ITranslator.hpp"
#include "JSON_Converter.hpp"

namespace JSON_Lib {

class XML_Translator : public ITranslator
{
public:
  // XML translator error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("XML Translator Error: " + message) {}
  };

  XML_Translator() = default;
  XML_Translator(const XML_Translator &other) = delete;
  XML_Translator &operator=(const XML_Translator &other) = delete;
  XML_Translator(XML_Translator &&other) = delete;
  XML_Translator &operator=(XML_Translator &&other) = delete;
  ~XML_Translator() override = default;

  // Convert to/from XML escaped characters
  std::string from([[maybe_unused]] const std::string &escapedString) const override { return escapedString; }

  std::string to(const std::string &rawString) const override
  {
    std::string translated;
    for (char16_t ch : toUtf16(rawString)) {
      if (isASCII(ch) && std::isprint(ch)) {
        if (ch == '&') {
          translated += "&amp;";
        } else if (ch == '<') {
          translated += "&lt;";
        } else if (ch == '>') {
          translated += "&gt;";
        } else if (ch == '\'') {
          translated += "&apos;";
        } else if (ch == '"') {
          translated += "&quot;";
        } else {
          translated += static_cast<unsigned char>(ch);
        }
      } else {
        translated += "&#x";
        const char *digits = "0123456789ABCDEF";
        translated += digits[ch >> 12 & 0x0f];
        translated += digits[ch >> 8 & 0x0f];
        translated += digits[ch >> 4 & 0x0f];
        translated += digits[ch&0x0f];
        translated += ";";
      }
    }
    return translated;
  };

private:

  /// <summary>
  /// Determine whether passed in character is vaid ASCII
  /// </summary>
  /// <param name="utf16Char">UTF16 character.</param>
  /// <returns>==true if valid ASCII.</returns>
  bool isASCII(char16_t utf16Char) const { return utf16Char > 0x001F && utf16Char < 0x0080; }
};
}// namespace JSON_Lib