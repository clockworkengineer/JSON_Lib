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
  ~XML_Stringify() override = default;

  void stringify(const JNode &jNode, IDestination &destination, long indent) const override;
  [[nodiscard]] long getIndent() const override;
  void setIndent(long indent) override;

private:
  // Stringify Bencode to XML
  void stringifyXML(const JNode &jNode, IDestination &destination, long indent) const;

  XML_Translator xmlTranslator;
  
};

}// namespace JSON_Lib
