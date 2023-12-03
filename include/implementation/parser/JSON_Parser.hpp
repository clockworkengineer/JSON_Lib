#pragma once

#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "JSON_Config.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class JSON_Parser : public IParser
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
  JNode parseString(ISource &source);
  JNode parseNumber(ISource &source);
  JNode parseBoolean(ISource &source);
  JNode parseNull(ISource &source);
  JNode parseObject(ISource &source);
  JNode parseArray(ISource &source);

  // Pointer to JSON translator interface
  ITranslator &jsonTranslator;
};

}// namespace JSON_Lib
