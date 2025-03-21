#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Default_Translator.hpp"

namespace JSON_Lib {

class JSON_Stringify final : public IStringify
{
public:
  explicit JSON_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
    : jsonTranslator(std::move(translator)) {}
  JSON_Stringify(const JSON_Stringify &other) = delete;
  JSON_Stringify &operator=(const JSON_Stringify &other) = delete;
  JSON_Stringify(JSON_Stringify &&other) = delete;
  JSON_Stringify &operator=(JSON_Stringify &&other) = delete;
  ~JSON_Stringify() override = default;

  /// <summary>
  /// Recursively traverse JNode structure encoding it into JSON string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">JNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified JSON.</param>
  /// <param name="indent">Current print indentation.</param>
  void stringify(const JNode &jNode, IDestination &destination, const unsigned long indent) const override
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
      throw Error("Unknown JNode type encountered during stringification.");
    }
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
  void stringifyObject(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    size_t commaCount = JRef<Object>(jNode).value().size() - 1;
    destination.add('{');
    if (indent != 0) { destination.add('\n'); }
    for (auto &entry : JRef<Object>(jNode).value()) {
      if (indent != 0) { destination.add(std::string(indent, ' ')); }
      stringify(entry.getKeyJNode(), destination, indent != 0 ? indent + printIndent : 0);
      destination.add(":");
      if (indent != 0) { destination.add(" "); }
      stringify(entry.getJNode(), destination, indent != 0 ? indent + printIndent : 0);
      if (commaCount-- > 0) {
        destination.add(",");
        if (indent != 0) { destination.add('\n'); }
      }
    }
    if (indent != 0) { destination.add("\n" + std::string(indent - printIndent, ' ')); }
    destination.add("}");
  }

  void stringifyArray(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    destination.add('[');
    if (!JRef<Array>(jNode).value().empty()) {
      size_t commaCount = JRef<Array>(jNode).value().size() - 1;
      if (indent != 0) { destination.add('\n'); }
      for (auto &entry : JRef<Array>(jNode).value()) {
        if (indent != 0) { destination.add(std::string(indent, ' ')); }
        stringify(entry, destination, indent != 0 ? indent + printIndent : 0);
        if (commaCount-- > 0) {
          destination.add(",");
          if (indent != 0) { destination.add('\n'); }
        }
      }
      if (indent != 0) { destination.add("\n" + std::string(indent - printIndent, ' ')); }
    }
    destination.add("]");
  }

  static void stringifyNumber(const JNode &jNode, IDestination &destination)
  {
    destination.add(JRef<Number>(jNode).toString());
  }

  static void stringifyBoolean(const JNode &jNode, IDestination &destination)
  {
    destination.add(JRef<Boolean>(jNode).toString());
  }

  static void stringifyNull([[maybe_unused]]const JNode &jNode, IDestination &destination) { destination.add(Null::toString()); }

  void stringifyString(const JNode &jNode, IDestination &destination) const
  {
    destination.add('"' + jsonTranslator->to(JRef<String>(jNode).toString()) + '"');
  }

  std::unique_ptr<ITranslator> jsonTranslator;

  inline static long printIndent{ 4 };
};

}// namespace JSON_Lib
