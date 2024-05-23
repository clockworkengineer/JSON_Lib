#pragma once


#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Translator.hpp"

namespace JSON_Lib {

class JSON_Stringify final : public IStringify
{
public:
  JSON_Stringify() = default;
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
void stringify(const JNode &jNode, IDestination &destination, const long indent) const override
  {
  if (jNode.isNumber()) {
    destination.add(JRef<Number>(jNode).toString());
  } else if (jNode.isString()) {
    destination.add('"');
    destination.add(jsonTranslator.to(JRef<String>(jNode).toString()));
    destination.add('"');
  } else if (jNode.isBoolean()) {
    destination.add(JRef<Boolean>(jNode).toString());
  } else if (jNode.isNull()) {
    destination.add(JRef<Null>(jNode).toString());
  } else if (jNode.isHole()) {
    destination.add(JRef<Hole>(jNode).toString());
  } else if (jNode.isObject()) {
    size_t commaCount = JRef<Object>(jNode).value().size() - 1;
    destination.add('{');
    if (indent != 0) { destination.add('\n'); }
    for (auto &entry : JRef<Object>(jNode).value()) {
      if (indent != 0) { destination.add(std::string(indent, ' ')); }
      destination.add('"');
      destination.add(jsonTranslator.to(JRef<String>(entry.getKey()).toString()));
      destination.add('"');
      destination.add(":");
      if (indent != 0) { destination.add(" "); }
      stringify(entry.getJNode(), destination, indent != 0 ? indent + printIndent : 0);
      if (commaCount-- > 0) {
        destination.add(",");
        if (indent != 0) { destination.add('\n'); }
      }
    }
    if (indent != 0) {
      destination.add('\n');
      destination.add(std::string(indent - printIndent, ' '));
    }
    destination.add("}");
  } else if (jNode.isArray()) {
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
      if (indent != 0) {
        destination.add('\n');
        destination.add(std::string(indent - printIndent, ' '));
      }
    }
    destination.add("]");
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

  JSON_Translator jsonTranslator;

  inline static long printIndent{ 4 };
};

}// namespace JSON_Lib
