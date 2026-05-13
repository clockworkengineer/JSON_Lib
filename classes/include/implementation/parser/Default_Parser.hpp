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

  // Get/Set parser max recursion depth (instance — not shared across JSON objects)
  void setMaxParserDepth(unsigned long depth) override { m_maxParserDepth = depth; }
  unsigned long getMaxParserDepth() const noexcept override { return m_maxParserDepth; }

  Node parse(ISource &source) override;
  Result<Node> parseResult(ISource &source) override;

private:
  // Parse JSON
  JSON_LIB_NODISCARD static Object::Entry
    parseObjectEntry(ISource &source, const ITranslator &translator, unsigned long parserDepth, unsigned long maxDepth);
  JSON_LIB_NODISCARD static Node
    parseString(ISource &source, const ITranslator &translator, unsigned long parserDepth);
  JSON_LIB_NODISCARD static Node parseNumber(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  JSON_LIB_NODISCARD static Node parseBoolean(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  JSON_LIB_NODISCARD static Node parseNull(ISource &source,
    const ITranslator &translator,
    unsigned long parserDepth);
  JSON_LIB_NODISCARD static Node parseObject(ISource &source, const ITranslator &translator, unsigned long parserDepth, unsigned long maxDepth);
  JSON_LIB_NODISCARD static Node parseArray(ISource &source, const ITranslator &translator, unsigned long parserDepth, unsigned long maxDepth);
  JSON_LIB_NODISCARD static Node parseNodes(ISource &source, const ITranslator &translator, unsigned long parserDepth, unsigned long maxDepth);

  // Reference to JSON translator interface
  const ITranslator &jsonTranslator;
  // Maximum parser depth (per-instance — not shared across JSON objects)
  unsigned long m_maxParserDepth{ kDefaultMaxParserDepth };
};

}// namespace JSON_Lib
