#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Translator.hpp"

namespace JSON_Lib {

class JSON_Parser final : public IParser
{
public:
  explicit JSON_Parser(ITranslator &translator) : jsonTranslator(translator) {}
  JSON_Parser(const JSON_Parser &other) = delete;
  JSON_Parser &operator=(const JSON_Parser &other) = delete;
  JSON_Parser(JSON_Parser &&other) = delete;
  JSON_Parser &operator=(JSON_Parser &&other) = delete;
  ~JSON_Parser() override = default;

  JNode parse(ISource &source) override;

private:
  // Parse JSON
  [[nodiscard]] static Object::Entry parseObjectEntry(ISource &source, const ITranslator &translator);
  [[nodiscard]] static JNode parseString(ISource &source, const ITranslator &translator);
  [[nodiscard]] static JNode parseNumber(ISource &source, [[maybe_unused]] const ITranslator &translator);
  [[nodiscard]] static JNode parseBoolean(ISource &source, [[maybe_unused]] const ITranslator &translator);
  [[nodiscard]] static JNode parseNull(ISource &source, [[maybe_unused]] const ITranslator &translator);
  [[nodiscard]] static JNode parseObject(ISource &source, const ITranslator &translator);
  [[nodiscard]] static JNode parseArray(ISource &source, const ITranslator &translator);
  [[nodiscard]] static JNode parseTree(ISource &source, const ITranslator &translator);
  // Parser routing table
  using ParseFunc = std::function<JNode(ISource &, const ITranslator &)>;
  inline static std::map<char, ParseFunc> parsers = { { '{', parseObject },
    { '[', parseArray },
    { 't', parseBoolean },
    { 'f', parseBoolean },
    { 'n', parseNull },
    { '"', parseString },
    { '\'', parseString },
    { '+', parseNumber },
    { '-', parseNumber },
    { '0', parseNumber },
    { '1', parseNumber },
    { '2', parseNumber },
    { '3', parseNumber },
    { '4', parseNumber },
    { '5', parseNumber },
    { '6', parseNumber },
    { '7', parseNumber },
    { '8', parseNumber },
    { '9', parseNumber } };

  // Reference to JSON translator interface
  const ITranslator &jsonTranslator;
};

}// namespace JSON_Lib
