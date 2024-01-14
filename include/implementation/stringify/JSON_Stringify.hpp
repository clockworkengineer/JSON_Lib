#pragma once

#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class JSON_Stringify : public IStringify
{
public:
  explicit JSON_Stringify(ITranslator &translator) : jsonTranslator(translator) {}
  JSON_Stringify(const JSON_Stringify &other) = delete;
  JSON_Stringify &operator=(const JSON_Stringify &other) = delete;
  JSON_Stringify(JSON_Stringify &&other) = delete;
  JSON_Stringify &operator=(JSON_Stringify &&other) = delete;
  ~JSON_Stringify() = default;

  virtual void stringify(const JNode &jNode, IDestination &destination, long indent) const override;

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
