#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Char_Constants.hpp"

namespace JSON_Lib {

class Default_Parser final : public IParser
{
public:
  // Default maximum parser depth
  constexpr static unsigned long kDefaultMaxParserDepth = 10;

  explicit Default_Parser(ITranslator &translator) : jsonTranslator(translator) {}
  Default_Parser(const Default_Parser &other) = delete;
  Default_Parser &operator=(const Default_Parser &other) = delete;
  Default_Parser(Default_Parser &&other) = delete;
  Default_Parser &operator=(Default_Parser &&other) = delete;
  ~Default_Parser() override = default;

  // Get/Set parser max recursion depth
  static void setMaxParserDepth(const unsigned long depth) { maxParserDepth = depth; }
  static unsigned long getMaxParserDepth() { return maxParserDepth; }

  Node parse(ISource &source) override;

private:
  // Parse JSON
  [[nodiscard]] static Object::Entry
    parseObjectEntry(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node
    parseString(ISource &source, const ITranslator &translator, [[maybe_unused]] unsigned long parserDepth);
  [[nodiscard]] static Node parseNumber(ISource &source,
    [[maybe_unused]] const ITranslator &translator,
    [[maybe_unused]] unsigned long parserDepth);
  [[nodiscard]] static Node parseBoolean(ISource &source,
    [[maybe_unused]] const ITranslator &translator,
    [[maybe_unused]] unsigned long parserDepth);
  [[nodiscard]] static Node parseNull(ISource &source,
    [[maybe_unused]] const ITranslator &translator,
    [[maybe_unused]] unsigned long parserDepth);
  [[nodiscard]] static Node parseObject(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node parseArray(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node parseNodes(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  // Parser routing table
  using ParseFunc = std::function<Node(ISource &, const ITranslator &, unsigned long parserDepth)>;
  inline static std::map<char, ParseFunc> parsers = { { JSON_Lib::kObjectBegin, parseObject },
    { JSON_Lib::kArrayBegin, parseArray },
    { 't', parseBoolean },
    { 'f', parseBoolean },
    { 'n', parseNull },
    { JSON_Lib::kStringQuote, parseString },
    { JSON_Lib::kStringSingleQuote, parseString },
    { JSON_Lib::kPlus, parseNumber },
    { JSON_Lib::kMinus, parseNumber },
    { JSON_Lib::kZero, parseNumber },
    { JSON_Lib::kOne, parseNumber },
    { JSON_Lib::kTwo, parseNumber },
    { JSON_Lib::kThree, parseNumber },
    { JSON_Lib::kFour, parseNumber },
    { JSON_Lib::kFive, parseNumber },
    { JSON_Lib::kSix, parseNumber },
    { JSON_Lib::kSeven, parseNumber },
    { JSON_Lib::kEight, parseNumber },
    { JSON_Lib::kNine, parseNumber } };

  // Reference to JSON translator interface
  const ITranslator &jsonTranslator;
  // Maximum parser depth
  inline static unsigned long maxParserDepth{ kDefaultMaxParserDepth };
};

}// namespace JSON_Lib
