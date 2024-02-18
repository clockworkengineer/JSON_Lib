#pragma once

#include <unordered_map>

#include "IConverter.hpp"
#include "ITranslator.hpp"

namespace JSON_Lib {

class JSON_Translator : public ITranslator
{
public:
  // JSON translator error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JSON Translator Error: " + message) {}
  };

  // Pass in user defined converter here
  explicit JSON_Translator(const IConverter &converter);
  // No other constructors supported
  JSON_Translator(const JSON_Translator &other) = delete;
  JSON_Translator &operator=(const JSON_Translator &other) = delete;
  JSON_Translator(JSON_Translator &&other) = delete;
  JSON_Translator &operator=(JSON_Translator &&other) = delete;
  ~JSON_Translator() override = default;

  // Convert to/from JSON escaped characters
  std::string from(const std::string &jsonString) override;
  std::string to(const std::string &utf8String) override;

private:
  // Character converter
  const IConverter &jsonConverter;
  // To/From escape sequence lookup maps
  std::unordered_map<char, char16_t> fromEscape;
  std::unordered_map<char16_t, char> toEscape;
};
}// namespace JSON_Lib
