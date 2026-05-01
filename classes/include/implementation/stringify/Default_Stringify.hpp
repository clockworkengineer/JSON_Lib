#pragma once

#include <memory>
#include <string_view>
#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_StringUtils.hpp"

namespace JSON_Lib {

class Default_Stringify final : public IStringify
{
public:
  explicit Default_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
    : jsonTranslator(std::move(translator))
  {
  }
  Default_Stringify(const Default_Stringify &other) = delete;
  Default_Stringify &operator=(const Default_Stringify &other) = delete;
  Default_Stringify(Default_Stringify &&other) = delete;
  Default_Stringify &operator=(Default_Stringify &&other) = delete;
  ~Default_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into JSON string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified JSON.</param>
  /// <param name="indent">Current print indentation.</param>
  void stringify(const Node &jNode, IDestination &destination, const unsigned long indent) const override
  {
    stringifyNodes(jNode, destination, indent);
  }

  // Set print ident value
  void setIndent(const long indent) override { setDefaultIndent(indent); }
  //  Set indent value
  long getIndent() const override { return defaultPrintIndent; }

  static void setDefaultIndent(const long indent)
  {
    if (indent < 0) { throw JSON_Lib::Error("Invalid print indentation value."); }
    defaultPrintIndent = indent;
  }

private:
  void stringifyNodes(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    jNode.visit(overloaded{
      [&](const Number &) { stringifyNumber(jNode, destination); },
      [&](const String &) { stringifyString(jNode, destination); },
      [&](const Boolean &) { stringifyBoolean(jNode, destination); },
      [&](const Null &) { stringifyNull(jNode, destination); },
      [&](const Hole &) { destination.add(Hole::toString()); },
      [&](const Object &) { stringifyObject(jNode, destination, indent); },
      [&](const Array &) { stringifyArray(jNode, destination, indent); },
      [&](const std::monostate &) { throw Error("Unknown Node type encountered during stringification."); }
    });
  }



  void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    const auto &entries = NRef<Object>(jNode).value();
    const bool pretty = indent != 0;
    size_t commaCount = entries.empty() ? 0 : entries.size() - 1;
    destination.add('{');
    if (pretty) { destination.add('\n'); }
    for (auto &entry : entries) {
      if (pretty) { addIndent(destination, indent); }
      stringifyString(entry.getKey(), destination);
      destination.add(':');
      if (pretty) { destination.add(' '); }
      stringifyNodes(entry.getNode(), destination, pretty ? indent + defaultPrintIndent : 0);
      addCommaNewline(destination, pretty, commaCount);
    }
    addPrettyTrailer(destination, pretty, indent, static_cast<unsigned long>(defaultPrintIndent));
    destination.add('}');
  }

  void stringifyArray(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    const auto &elements = NRef<Array>(jNode).value();
    const bool pretty = indent != 0;
    destination.add('[');
    if (!elements.empty()) {
      size_t commaCount = elements.size() - 1;
      if (pretty) { destination.add('\n'); }
      for (auto &entry : elements) {
        if (pretty) { addIndent(destination, indent); }
        stringifyNodes(entry, destination, pretty ? indent + defaultPrintIndent : 0);
        addCommaNewline(destination, pretty, commaCount);
      }
      addPrettyTrailer(destination, pretty, indent, static_cast<unsigned long>(defaultPrintIndent));
    }
    destination.add(']');
  }

  static void stringifyNumber(const Node &jNode, IDestination &destination)
  {
    destination.add(NRef<Number>(jNode).toString());
  }
  static void stringifyBoolean(const Node &jNode, IDestination &destination)
  {
    destination.add(NRef<Boolean>(jNode).toString());
  }
  static void stringifyNull(const Node &, IDestination &destination)
  {
    destination.add("null");
  }
  void stringifyString(const Node &jNode, IDestination &destination) const
  {
    appendQuotedString(NRef<String>(jNode).value(), destination, *jsonTranslator);
  }
  void stringifyString(const std::string_view &value, IDestination &destination) const
  {
    appendQuotedString(value, destination, *jsonTranslator);
  }

  std::unique_ptr<ITranslator> jsonTranslator;
  inline static long defaultPrintIndent{ 4 };
};

}// namespace JSON_Lib
