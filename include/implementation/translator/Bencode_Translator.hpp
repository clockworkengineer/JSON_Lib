#pragma once

#include "IConverter.hpp"
#include "ITranslator.hpp"

namespace JSON_Lib {

class Bencode_Translator : public ITranslator
{
public:
  // Bencode translator error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("Bencode Translator Error: " + message) {}
  };

  // Pass in user defined converter here
  explicit Bencode_Translator(const IConverter &converter) : bencodeConverter(converter) {}
  // No other constructors supported
  Bencode_Translator(const Bencode_Translator &other) = delete;
  Bencode_Translator &operator=(const Bencode_Translator &other) = delete;
  Bencode_Translator(Bencode_Translator &&other) = delete;
  Bencode_Translator &operator=(Bencode_Translator &&other) = delete;
  ~Bencode_Translator() override = default;

  // Convert to/from Bencode escaped characters
  std::string from([[maybe_unused]] const std::string &escapedString) override { return (escapedString); }
  std::string to(const std::string &rawString) override { return (rawString); };

private:
  // Character converter
  const IConverter &bencodeConverter;
};
}// namespace JSON_Lib