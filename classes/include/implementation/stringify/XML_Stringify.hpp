#pragma once

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

  /// <summary>
  /// Recursively traverse JNode structure encoding it into JSON string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">JNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified JSON.</param>
  /// <param name="indent">Current print indentation.</param>
  void stringify(const JNode &jNode, IDestination &destination, [[maybe_unused]] long indent) const override
  {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    stringifyXML(jNode, destination, 0);
    destination.add("</root>");
  }

private:
  /// <summary>
  /// Recursively traverse JNode structure encoding it into XML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">JNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified XML.</param>
  /// <param name="indent">Current print indentation.</param>
  void stringifyXML(const JNode &jNode, IDestination &destination, const long indent) const
  {
    if (isA<Number>(jNode)) {
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
    } else if (isA<Object>(jNode)) {
      for (const auto &jNodeNext : JRef<Object>(jNode).value()) {
        auto elementName = JRef<String>(jNodeNext.getKey()).value();
        std::ranges::replace(elementName, ' ', '-');
        destination.add("<" + elementName + ">");
        stringifyXML(jNodeNext.getJNode(), destination, 0);
        destination.add("</" + elementName + ">");
      }
    } else if (isA<Array>(jNode)) {
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

  XML_Translator xmlTranslator;
};

}// namespace JSON_Lib
