#pragma once

#include <memory>
#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Translator.hpp"
#include "JSON_StringifierBase.hpp"

namespace JSON_Lib {

class XML_Stringify final : public StringifierBase
{
public:
  explicit XML_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<XML_Translator>())
    : StringifierBase(std::move(translator))
  {
  }

  /// <summary>
  /// Recursively traverse Node structure encoding it into XML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified XML.</param>
  /// <param name="indent">Current print indentation.</param>
  void
    stringify(const Node &jNode, IDestination &destination, const unsigned long) const override
  {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    stringifyNodes(jNode, destination, 0);
    destination.add("</root>");
  }

private:
  void stringifyNodes(const Node &jNode, IDestination &destination, const long) const
  {
    jNode.visit(overloaded{
      [&](const Number &) { stringifyNumber(jNode, destination); },
      [&](const String &) { stringifyString(jNode, destination); },
      [&](const Boolean &) { stringifyBoolean(jNode, destination); },
      [&](const Null &) { stringifyNull(jNode, destination); },
      [&](const Hole &) { stringifyNull(jNode, destination); },
      [&](const Object &) { stringifyObject(jNode, destination, 0); },
      [&](const Array &) { stringifyArray(jNode, destination, 0); },
      [&](const std::monostate &) { throw Error("Unknown Node type encountered during stringification."); }
    });
  }
  void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long) const
  {
    for (const auto &jNodeNext : NRef<Object>(jNode).value()) {
      std::string elementName { jNodeNext.getKey()};
      std::ranges::replace(elementName, ' ', '-');
      destination.add('<');
      destination.add(elementName);
      destination.add('>');
      stringifyNodes(jNodeNext.getNode(), destination, 0);
      destination.add("</");
      destination.add(elementName);
      destination.add('>');
    }
  }
  void stringifyArray(const Node &jNode, IDestination &destination, const unsigned long) const
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
  static void stringifyNull(const Node &, IDestination &) {}
  void stringifyString(const Node &jNode, IDestination &destination) const
  {
    destination.add(translator_->to(NRef<String>(jNode).value()));
  }
};

}// namespace JSON_Lib
