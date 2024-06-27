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
  [[nodiscard]] static Object::Entry parseObjectEntry(ISource &source,const ITranslator &translator);
  [[nodiscard]] static JNode parseString(ISource &source, const  ITranslator &translator);
  [[nodiscard]] static JNode parseNumber(ISource &source);
  [[nodiscard]] static JNode parseBoolean(ISource &source);
  [[nodiscard]] static JNode parseNull(ISource &source);
  [[nodiscard]] static JNode parseObject(ISource &source,const ITranslator &translator);
  [[nodiscard]] static JNode parseArray(ISource &source, const ITranslator &translator);
  [[nodiscard]] static JNode parseTree(ISource &source, const ITranslator &translator);

  // Reference to JSON translator interface
   const ITranslator &jsonTranslator;
};

}// namespace JSON_Lib
