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

class Default_Stringify : public IStringify
{
public:
  explicit Default_Stringify(ITranslator &translator) : jsonTranslator(translator) {}
  Default_Stringify(const Default_Stringify &other) = delete;
  Default_Stringify &operator=(const Default_Stringify &other) = delete;
  Default_Stringify(Default_Stringify &&other) = delete;
  Default_Stringify &operator=(Default_Stringify &&other) = delete;
  ~Default_Stringify() = default;

  virtual void stringify(const JNode &jNode, IDestination &destination, long indent) const;

  // Set print ident value
  void setIndent(long indent)
  {
    if (indent < 0) { throw JSON::Error("Invalid print indentation value."); };
    printIndent = indent;
  }

  long getIndent() const { return (printIndent); }

private:
  // Stringify JSON
  void stringifyNumber(const JNode &jNode, IDestination &destination) const;
  void stringifyString(const JNode &jNode, IDestination &destination) const;
  void stringifyBoolean(const JNode &jNode, IDestination &destination) const;
  void stringifyNull(const JNode &jNode, IDestination &destination) const;
  void stringifyHole(const JNode &jNode, IDestination &destination) const;
  void stringifyObject(const JNode &jNode, IDestination &destination, long indent) const;
  void stringifyArray(const JNode &jNode, IDestination &destination, long indent) const;

  ITranslator &jsonTranslator;

  inline static long printIndent{ 4 };
};

}// namespace JSON_Lib
