//
// Class: XML_Stringify
//
// Description: Stringify JNode tree to XML on destination stream.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Stringify.hpp"

namespace JSON_Lib {

/// <summary>
/// Recursively traverse JNode structure encoding it into XML string on
/// the destination stream passed in.
/// </summary>
/// <param name="jNode">JNode structure to be traversed.</param>
/// <param name="destination">Destination stream for stringified XML.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyXML(const JNode &jNode, IDestination &destination, const long indent) const
{
  if (jNode.isNumber()) {
    destination.add(std::to_string(JRef<Number>(jNode).value<int>()));
  } else if (jNode.isString()) {
    destination.add(xmlTranslator.to(JRef<String>(jNode).value()));
  } else if (jNode.isBoolean()) {
    if (JRef<Boolean>(jNode).value()) {
      destination.add("True");
    } else {
      destination.add("False");
    }
  } else if (jNode.isNull()||jNode.isHole()) {
    ;
  } else if (jNode.isObject()) {
    for (const auto &jNodeNext : JRef<Object>(jNode).value()) {
      auto elementName = JRef<String>(jNodeNext.getKey()).value();
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      stringifyXML(jNodeNext.getJNode(), destination, 0);
      destination.add("</" + elementName + ">");
    }
  } else if (jNode.isArray()) {
    if (JRef<Array>(jNode).value().size() > 1) {
      for (const auto &bNodeNext : JRef<Array>(jNode).value()) {
        destination.add("<Row>");
        stringifyXML(bNodeNext, destination, 0);
        destination.add("</Row>");
      }
    }
  } else {
    throw Error("Unknown JNode type encountered during stringification.");
  }
}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name="jNode">JNode structure to be traversed.</param>
/// <param name="destination">Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringify(const JNode &jNode, IDestination &destination, [[maybe_unused]] long indent) const
{
  destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
  destination.add("<root>");
  stringifyXML(jNode, destination, 0);
  destination.add("</root>");
}

long XML_Stringify::getIndent() const { return 0; }

void XML_Stringify::setIndent([[maybe_unused]] long indent) {}

}// namespace JSON_Lib
