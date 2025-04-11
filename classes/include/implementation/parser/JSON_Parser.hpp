#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Converter.hpp"
#include "Default_Translator.hpp"

namespace JSON_Lib {

class JSON_Parser final : public IParser
{
public:
  // Default maximum parser depth
  constexpr static unsigned long kDefaultMaxParserDepth = 10;

  explicit JSON_Parser(ITranslator &translator) : jsonTranslator(translator) {}
  JSON_Parser(const JSON_Parser &other) = delete;
  JSON_Parser &operator=(const JSON_Parser &other) = delete;
  JSON_Parser(JSON_Parser &&other) = delete;
  JSON_Parser &operator=(JSON_Parser &&other) = delete;
  ~JSON_Parser() override = default;

  // Get/Set parser max recursion depth
  static void setMaxParserDepth(const unsigned long depth) { maxParserDepth = depth; }
  static unsigned long getMaxParserDepth() { return maxParserDepth; }

  JNode parse(ISource &source) override;

private:
  // Parse JSON
  [[nodiscard]] static Object::Entry parseObjectEntry(ISource &source, const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseString(ISource &source, const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseNumber(ISource &source, [[maybe_unused]] const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseBoolean(ISource &source, [[maybe_unused]] const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseNull(ISource &source, [[maybe_unused]] const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseObject(ISource &source, const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseArray(ISource &source, const ITranslator &translator, const unsigned long parserDepth);
  [[nodiscard]] static JNode parseJNodes(ISource &source, const ITranslator &translator, const unsigned long parserDepth);
  // Parser routing table
  using ParseFunc = std::function<JNode(ISource &, const ITranslator &, const unsigned long parserDepth)>;
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
  // Maximum parser depth
  inline static unsigned long maxParserDepth {kDefaultMaxParserDepth};

};

}// namespace JSON_Lib
