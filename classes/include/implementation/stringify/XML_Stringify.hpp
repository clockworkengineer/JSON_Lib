#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Translator.hpp"

namespace JSON_Lib {

class XML_Stringify final : public IStringify
{
public:
  explicit XML_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<XML_Translator>())
    {xmlTranslator = std::move(translator);}
  XML_Stringify(const XML_Stringify &other) = delete;
  XML_Stringify &operator=(const XML_Stringify &other) = delete;
  XML_Stringify(XML_Stringify &&other) = delete;
  XML_Stringify &operator=(XML_Stringify &&other) = delete;
  ~XML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into XML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified XML.</param>
  /// <param name="indent">Current print indentation.</param>
  void
    stringify(const Node &jNode, IDestination &destination, [[maybe_unused]] const unsigned long indent) const override
  {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    stringifyNodes(jNode, destination, 0);
    destination.add("</root>");
  }

private:
  static void stringifyNodes(const Node &jNode, IDestination &destination, [[maybe_unused]] const long indent)
  {
    if (isA<Number>(jNode)) {
      stringifyNumber(jNode, destination);
    } else if (isA<String>(jNode)) {
      stringifyString(jNode, destination);
    } else if (isA<Boolean>(jNode)) {
      stringifyBoolean(jNode, destination);
    } else if (isA<Null>(jNode) || isA<Hole>(jNode)) {
      stringifyNull(jNode, destination);
    } else if (isA<Object>(jNode)) {
      stringifyObject(jNode, destination, 0);
    } else if (isA<Array>(jNode)) {
      stringifyArray(jNode, destination, 0);
    } else {
      throw Error("Unknown Node type encountered during stringification.");
    }
  }
  static void stringifyObject(const Node &jNode, IDestination &destination, [[maybe_unused]] const unsigned long indent)
  {
    for (const auto &jNodeNext : NRef<Object>(jNode).value()) {
      std::string elementName { jNodeNext.getKey()} ;
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      stringifyNodes(jNodeNext.getNode(), destination, 0);
      destination.add("</" + elementName + ">");
    }
  }
  static void stringifyArray(const Node &jNode, IDestination &destination, [[maybe_unused]]const unsigned long indent)
  {
    if (NRef<Array>(jNode).value().size() > 1) {
      for (const auto &bNodeNext : NRef<Array>(jNode).value()) {
        destination.add("<Row>");
        stringifyNodes(bNodeNext, destination, 0);
        destination.add("</Row>");
      }
    }
  }
  static void stringifyNumber(const Node &jNode, IDestination &destination)
  {
    destination.add(std::to_string(NRef<Number>(jNode).value<long long>()));
  }
  static void stringifyBoolean(const Node &jNode, IDestination &destination)
  {
    if (NRef<Boolean>(jNode).value()) {
      destination.add("True");
    } else {
      destination.add("False");
    }
  }
  static void stringifyNull([[maybe_unused]]const Node &jNode, [[maybe_unused]]IDestination &destination) {}
  static void stringifyString(const Node &jNode, IDestination &destination)
  {
    destination.add(xmlTranslator->to(NRef<String>(jNode).value()));
  }

  inline static std::unique_ptr<ITranslator> xmlTranslator;
};

}// namespace JSON_Lib
