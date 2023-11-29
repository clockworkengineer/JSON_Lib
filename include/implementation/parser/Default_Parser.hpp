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

class Default_Parser : public IParser
{
public:
  explicit Default_Parser(ITranslator &translator) : jsonTranslator(translator) {}
  Default_Parser(const Default_Parser &other) = delete;
  Default_Parser &operator=(const Default_Parser &other) = delete;
  Default_Parser(Default_Parser &&other) = delete;
  Default_Parser &operator=(Default_Parser &&other) = delete;
  ~Default_Parser() = default;

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
