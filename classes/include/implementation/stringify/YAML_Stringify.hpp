


#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Default_Translator.hpp"

namespace JSON_Lib {

class YAML_Stringify final : public IStringify
{
public:
  // Constructors/destructors
  YAML_Stringify() = default;
  YAML_Stringify(const YAML_Stringify &other) = delete;
  YAML_Stringify &operator=(const YAML_Stringify &other) = delete;
  YAML_Stringify(YAML_Stringify &&other) = delete;
  YAML_Stringify &operator=(YAML_Stringify &&other) = delete;
  ~YAML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse JNode structure encoding it into YAML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">JNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified YAML.</param>
  /// <param name="indent">Current print indentation.</param>
  void
    stringify(const JNode &jNode, IDestination &destination, [[maybe_unused]] const unsigned long indent) const override
  {
    destination.add("---\n");
    stringifyYAML(jNode, destination, 0);
    destination.add("...\n");
  }

private:
  static auto calculateIndent(IDestination &destination, const unsigned long indent)
  {
    if (destination.last() == '\n') { return std::string(indent, ' '); }
    return std::string("");
  }
  void stringifyYAML(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    if (isA<Object>(jNode)) {
      stringifyObject(jNode, destination, indent);
    } else if (isA<Array>(jNode)) {
      stringifyArray(jNode, destination, indent);
    } else if (isA<Number>(jNode)) {
      stringifyNumber(jNode, destination);
    } else if (isA<String>(jNode)) {
      stringifyString(jNode, destination);
    } else if (isA<Boolean>(jNode)) {
      stringifyBoolean(jNode, destination);
    } else if (isA<Null>(jNode)) {
      stringifyNull(jNode, destination);
    }
  }

  void stringifyObject(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    if (!JRef<Object>(jNode).value().empty()) {
      for (const auto &entryJNode : JRef<Object>(jNode).value()) {
        destination.add(calculateIndent(destination, indent));
        destination.add("\"" + yamlTranslator.to(JRef<String>(entryJNode.getKeyJNode()).value()) + "\"");
        destination.add(": ");
        if (isA<Array>(entryJNode.getJNode()) || isA<Object>(entryJNode.getJNode())) { destination.add('\n'); }
        stringifyYAML(entryJNode.getJNode(), destination, indent + 2);
      }
    } else {
      destination.add("{}\n");
    }
  }

  void stringifyArray(const JNode &jNode, IDestination &destination, const unsigned long indent) const
  {
    std::string spaces(indent, ' ');
    if (!JRef<Array>(jNode).value().empty()) {
      for (const auto &jNodeNext : JRef<Array>(jNode).value()) {
        destination.add(calculateIndent(destination, indent) + "- ");
        stringifyYAML(jNodeNext, destination, indent + 2);
      }
    } else {
      destination.add("[]\n");
    }
  }

  static void stringifyNumber(const JNode &jNode, IDestination &destination)
  {
    destination.add(JRef<Number>(jNode).toString() + "\n");
  }

  static void stringifyBoolean(const JNode &jNode, IDestination &destination)
  {
    destination.add(JRef<Boolean>(jNode).toString() + "\n");
  }

  static void stringifyNull(const JNode &jNode, IDestination &destination)
  {
    destination.add(JRef<Null>(jNode).toString() + "\n");
  }

  void stringifyString(const JNode &jNode, IDestination &destination) const
  {
    destination.add("\"" + yamlTranslator.to(JRef<String>(jNode).value()) + "\"" + "\n");
  }

  Default_Translator yamlTranslator;
};
}// namespace JSON_Lib
