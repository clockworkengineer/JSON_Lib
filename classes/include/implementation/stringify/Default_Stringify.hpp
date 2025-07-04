#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class Default_Stringify final : public IStringify
{
public:
  explicit Default_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
    {jsonTranslator=std::move(translator);}
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
    stringifyNodes(jNode,destination, indent);
  }

  // Set print ident value
  void setIndent(const long indent) override
  {
    if (indent < 0) { throw JSON_Lib::Error("Invalid print indentation value."); }
    printIndent = indent;
  }
  //  Set indent value
  long getIndent() const override { return printIndent; }

private:
  static void stringifyNodes(const Node &jNode, IDestination &destination, const unsigned long indent)
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
  static void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long indent)
  {
    size_t commaCount = JRef<Object>(jNode).value().size() - 1;
    destination.add('{');
    if (indent != 0) { destination.add('\n'); }
    for (auto &entry : JRef<Object>(jNode).value()) {
      if (indent != 0) { destination.add(std::string(indent, ' ')); }
      stringifyNodes(entry.getKeyNode(), destination, indent != 0 ? indent + printIndent : 0);
      destination.add(":");
      if (indent != 0) { destination.add(" "); }
      stringifyNodes(entry.getNode(), destination, indent != 0 ? indent + printIndent : 0);
      if (commaCount-- > 0) {
        destination.add(",");
        if (indent != 0) { destination.add('\n'); }
      }
    }
    if (indent != 0) { destination.add("\n" + std::string(indent - printIndent, ' ')); }
    destination.add("}");
  }
  static void stringifyArray(const Node &jNode, IDestination &destination, const unsigned long indent)
  {
    destination.add('[');
    if (!JRef<Array>(jNode).value().empty()) {
      size_t commaCount = JRef<Array>(jNode).value().size() - 1;
      if (indent != 0) { destination.add('\n'); }
      for (auto &entry : JRef<Array>(jNode).value()) {
        if (indent != 0) { destination.add(std::string(indent, ' ')); }
        stringifyNodes(entry, destination, indent != 0 ? indent + printIndent : 0);
        if (commaCount-- > 0) {
          destination.add(",");
          if (indent != 0) { destination.add('\n'); }
        }
      }
      if (indent != 0) { destination.add("\n" + std::string(indent - printIndent, ' ')); }
    }
    destination.add("]");
  }
  static void stringifyNumber(const Node &jNode, IDestination &destination)
  {
    destination.add(JRef<Number>(jNode).toString());
  }
  static void stringifyBoolean(const Node &jNode, IDestination &destination)
  {
    destination.add(JRef<Boolean>(jNode).toString());
  }
  static void stringifyNull([[maybe_unused]]const Node &jNode, IDestination &destination) { destination.add(Null::toString()); }
  static void stringifyString(const Node &jNode, IDestination &destination)
  {
    destination.add('"' + jsonTranslator->to(JRef<String>(jNode).toString()) + '"');
  }

  inline static std::unique_ptr<ITranslator> jsonTranslator;
  inline static long printIndent{ 4 };
};

}// namespace JSON_Lib
