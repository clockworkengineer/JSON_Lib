#pragma once

#include "IConverter.hpp"
#include "ITranslator.hpp"

namespace JSON_Lib {

class XML_Translator : public ITranslator
{
public:
  // XML translator error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("XML Translator Error: " + message) {}
  };

  // Pass in user defined converter here
  explicit XML_Translator(const IConverter &converter) : xmlConverter(converter) {}
  // No other constructors supported
  XML_Translator(const XML_Translator &other) = delete;
  XML_Translator &operator=(const XML_Translator &other) = delete;
  XML_Translator(XML_Translator &&other) = delete;
  XML_Translator &operator=(XML_Translator &&other) = delete;
  ~XML_Translator() override = default;

  // Convert to/from XML escaped characters
  std::string from([[maybe_unused]] const std::string &jsonString) override { return (jsonString); }

  std::string to(const std::string &utf8String) override
  {
    std::string translated;
    for (char16_t ch : xmlConverter.toUtf16(utf8String)) {
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
        std::string utf8Buffer;
        const char *digits = "0123456789ABCDEF";
        translated += digits[(ch >> 12) & 0x0f];
        translated += digits[(ch >> 8) & 0x0f];
        translated += digits[(ch >> 4) & 0x0f];
        translated += digits[(ch)&0x0f];
        translated += ";";
      }
    }
    return (translated);
  };

private:
  // Character converter
  const IConverter &xmlConverter;

  /// <summary>
  /// Determine whether passed in character is vaid ASCII
  /// </summary>
  /// <param name="utf16Char">UTF16 character.</param>
  /// <returns>==true if valid ASCII.</returns>
  bool isASCII(char16_t utf16Char) { return (((utf16Char > 0x001F) && (utf16Char < 0x0080))); }
};
}// namespace JSON_Lib