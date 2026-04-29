#pragma once

#include <memory>
#include <string_view>
#include "JSON.hpp"
#include "JSON_Core.hpp"

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
  void setIndent(const long indent) override
  {
    if (indent < 0) { throw JSON_Lib::Error("Invalid print indentation value."); }
    defaultPrintIndent = indent;
  }
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
    if (isA<Number>(jNode)) {
      stringifyNumber(jNode, destination);
    } else if (isA<String>(jNode)) {
      stringifyString(jNode, destination);
    } else if (isA<Boolean>(jNode)) {
      stringifyBoolean(jNode, destination);
    } else if (isA<Null>(jNode)) {
      stringifyNull(jNode, destination);
    } else if (isA<Hole>(jNode)) {
      destination.add(Hole::toString());
    } else if (isA<Object>(jNode)) {
      stringifyObject(jNode, destination, indent);
    } else if (isA<Array>(jNode)) {
      stringifyArray(jNode, destination, indent);
    } else {
      throw Error("Unknown Node type encountered during stringification.");
    }
  }

  static void addIndent(IDestination &destination, const unsigned long indent)
  {
    for (unsigned long count = 0; count < indent; ++count) {
      destination.add(' ');
    }
  }

  void appendQuotedString(const std::string_view &value, IDestination &destination) const
  {
    destination.add('"');
    destination.add(jsonTranslator->to(value));
    destination.add('"');
  }

  void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    const auto &entries = NRef<Object>(jNode).value();
    const bool pretty = indent != 0;
    const size_t commaCountStart = entries.size() > 0 ? entries.size() - 1 : 0;
    size_t commaCount = commaCountStart;

    destination.add('{');
    if (pretty) { destination.add('\n'); }
    for (auto &entry : entries) {
      if (pretty) { addIndent(destination, indent); }
      stringifyString(entry.getKey(), destination);
      destination.add(':');
      if (pretty) { destination.add(' '); }
      stringifyNodes(entry.getNode(), destination, pretty ? indent + defaultPrintIndent : 0);
      if (commaCount-- > 0) {
        destination.add(',');
        if (pretty) { destination.add('\n'); }
      }
    }
    if (pretty) {
      destination.add('\n');
      addIndent(destination, indent - defaultPrintIndent);
    }
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
        if (commaCount-- > 0) {
          destination.add(',');
          if (pretty) { destination.add('\n'); }
        }
      }
      if (pretty) {
        destination.add('\n');
        addIndent(destination, indent - defaultPrintIndent);
      }
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
    appendQuotedString(NRef<String>(jNode).value(), destination);
  }
  void stringifyString(const std::string_view &value, IDestination &destination) const
  {
    appendQuotedString(value, destination);
  }

  std::unique_ptr<ITranslator> jsonTranslator;
  inline static long defaultPrintIndent{ 4 };
};

}// namespace JSON_Lib
