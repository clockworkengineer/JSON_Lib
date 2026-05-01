#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Char_Constants.hpp"
#include "JSON_Config.hpp"

namespace JSON_Lib {

class Default_Parser final : public IParser
{
public:
  // Default maximum parser depth
#if JSON_LIB_MAX_PARSER_DEPTH
  constexpr static unsigned long kDefaultMaxParserDepth = JSON_LIB_MAX_PARSER_DEPTH;
#else
  constexpr static unsigned long kDefaultMaxParserDepth = 10;
#endif

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
  Result<Node> parseResult(ISource &source) override;

private:
  // Parse JSON
  [[nodiscard]] static Object::Entry
    parseObjectEntry(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node
    parseString(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node parseNumber(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  [[nodiscard]] static Node parseBoolean(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  [[nodiscard]] static Node parseNull(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  [[nodiscard]] static Node parseObject(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node parseArray(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  [[nodiscard]] static Node parseNodes(ISource &source, const ITranslator &translator, unsigned long parserDepth);

  // Reference to JSON translator interface
  const ITranslator &jsonTranslator;
  // Maximum parser depth
  inline static unsigned long maxParserDepth{ kDefaultMaxParserDepth };
};

}// namespace JSON_Lib
