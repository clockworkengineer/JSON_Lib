#pragma once

#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
  ~JSON_Parser() = default;

  virtual JNode parse(ISource &source);

private:
  // Parse JSON
  Object::Entry parseObjectEntry(ISource &source);
  JNode parseString(ISource &source) const;
  static JNode parseNumber(ISource &source);
  static JNode parseBoolean(ISource &source);
  static JNode parseNull(ISource &source);
  JNode parseObject(ISource &source);
  JNode parseArray(ISource &source);

  // Pointer to JSON translator interface
  ITranslator &jsonTranslator;
};

}// namespace JSON_Lib
