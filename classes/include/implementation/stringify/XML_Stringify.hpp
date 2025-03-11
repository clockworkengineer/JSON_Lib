#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Translator.hpp"

namespace JSON_Lib {

class XML_Stringify final : public IStringify
{
public:
  explicit XML_Stringify(std::unique_ptr<ITranslator> translator =
                            std::make_unique<XML_Translator>())
      : xmlTranslator(std::move(translator)) {};
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
  void stringify(const JNode &jNode, IDestination &destination, [[maybe_unused]] const unsigned long indent) const override
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
  void stringifyXML(const JNode &jNode, IDestination &destination, [[maybe_unused]] const long indent) const
  {
    if (isA<Number>(jNode)) {
stringifyNumber(jNode, destination);
    } else if (isA<String>(jNode)) {
      stringifyString(jNode, destination);
    } else if (isA<Boolean>(jNode)) {
      stringifyBoolean(jNode, destination);
    } else if (isA<Null>(jNode)||isA<Hole>(jNode)) {
      stringifyNull(jNode, destination);
    } else if (isA<Object>(jNode)) {
      stringifyObject(jNode, destination, 0);
    } else if (isA<Array>(jNode)) {
      stringifyArray(jNode, destination,0);
    } else {
      throw Error("Unknown JNode type encountered during stringification.");
    }
  }

  void stringifyObject(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    for (const auto &jNodeNext : JRef<Object>(jNode).value()) {
      auto elementName = jNodeNext.getKey();
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      stringifyXML(jNodeNext.getJNode(), destination, 0);
      destination.add("</" + elementName + ">");
    }
  }

  void stringifyArray(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    if (JRef<Array>(jNode).value().size() > 1) {
      for (const auto &bNodeNext : JRef<Array>(jNode).value()) {
        destination.add("<Row>");
        stringifyXML(bNodeNext, destination, 0);
        destination.add("</Row>");
      }
    }
  }

  static void stringifyNumber(const JNode &jNode, IDestination &destination)
  {
    destination.add(std::to_string(JRef<Number>(jNode).value<long long>()));
  }

  static void stringifyBoolean(const JNode &jNode, IDestination &destination)
  {
    if (JRef<Boolean>(jNode).value()) {
      destination.add("True");
    } else {
      destination.add("False");
    }
  }

  static void stringifyNull(const JNode &jNode, IDestination &destination)
  {
  }

  void stringifyString(const JNode &jNode, IDestination &destination) const
  {
    destination.add(xmlTranslator->to(JRef<String>(jNode).value()));
  }

  std::unique_ptr<ITranslator> xmlTranslator;
};

}// namespace JSON_Lib
