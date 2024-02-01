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
  XML_Translator() = default;
  // No other constructors supported
  XML_Translator(const XML_Translator &other) = delete;
  XML_Translator &operator=(const XML_Translator &other) = delete;
  XML_Translator(XML_Translator &&other) = delete;
  XML_Translator &operator=(XML_Translator &&other) = delete;
  ~XML_Translator() override = default;

  // Convert to/from XML escaped characters
  std::string from([[maybe_unused]] const std::string &jsonString) override { return (""); }

  std::string to(const std::string &utf8String) override
  {
    std::string translated;
    for (unsigned char ch : utf8String) {
      if (!isprint(ch)) {
        char escaped[5];
        translated += "&#x";
        sprintf_s(escaped, "%04x", ch);
        translated += escaped[0];
        translated += escaped[1];
        translated += escaped[2];
        translated += escaped[3];
        translated += ";";
      } else {
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
          translated += ch;
        }
      }
    }
    return (translated);
  };

private:
};
}// namespace JSON_Lib