#pragma once

#include <algorithm>

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Translator.hpp"

namespace JSON_Lib {

class XML_Stringify : public IStringify
{
public:
  XML_Stringify() = default;
  XML_Stringify(const XML_Stringify &other) = delete;
  XML_Stringify &operator=(const XML_Stringify &other) = delete;
  XML_Stringify(XML_Stringify &&other) = delete;
  XML_Stringify &operator=(XML_Stringify &&other) = delete;
  ~XML_Stringify() = default;

  virtual void stringify(const JNode &jNode, IDestination &destination, long indent) const override;
  virtual long getIndent() const override;
  virtual void setIndent(long indent) override;

private:
  // Stringify Bencode to XML
  void stringifyNumber(const JNode &jNode, IDestination &destination) const;
  void stringifyString(const JNode &jNode, IDestination &destination) const;
  void stringifyBoolean(const JNode &jNode, IDestination &destination) const;
  void stringifyNull(const JNode &jNode, IDestination &destination) const;
  void stringifyHole(const JNode &jNode, IDestination &destination) const;
  void stringifyObject(const JNode &jNode, IDestination &destination, long indent) const;
  void stringifyArray(const JNode &jNode, IDestination &destination, long indent) const;
  void stringifyXML(const JNode &jNode, IDestination &destination, long indent) const;

  XML_Translator xmlTranslator;
  
};

}// namespace JSON_Lib
