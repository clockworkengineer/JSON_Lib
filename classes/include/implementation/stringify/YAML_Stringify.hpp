#pragma once

#include <memory>
#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_StringUtils.hpp"

namespace JSON_Lib {

class YAML_Stringify final : public IStringify
{
public:
  // Constructors/destructors
  explicit YAML_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
     : yamlTranslator(std::move(translator))
  {
  }
  YAML_Stringify(const YAML_Stringify &other) = delete;
  YAML_Stringify &operator=(const YAML_Stringify &other) = delete;
  YAML_Stringify(YAML_Stringify &&other) = delete;
  YAML_Stringify &operator=(YAML_Stringify &&other) = delete;
  ~YAML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into YAML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified YAML.</param>
  /// <param name="indent">Current print indentation.</param>
  void
    stringify(const Node &jNode, IDestination &destination, const unsigned long) const override
  {
    destination.add("---\n");
    stringifyNodes(jNode, destination, 0);
    destination.add("...\n");
  }

private:
  void stringifyNodes(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    jNode.visit(overloaded{
      [&](const Object &) { stringifyObject(jNode, destination, indent); },
      [&](const Array &) { stringifyArray(jNode, destination, indent); },
      [&](const Number &) { stringifyNumber(jNode, destination); },
      [&](const String &) { stringifyString(jNode, destination); },
      [&](const Boolean &) { stringifyBoolean(jNode, destination); },
      [&](const Null &) { stringifyNull(jNode, destination); },
      [&](const auto &) {}
    });
  }
  void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    if (!NRef<Object>(jNode).value().empty()) {
      for (const auto &entryNode : NRef<Object>(jNode).value()) {
        addIndentIfNewline(destination, indent);
        destination.add('"');
        destination.add(yamlTranslator->to(entryNode.getKey()));
        destination.add("\": ");
        if (isA<Array>(entryNode.getNode()) || isA<Object>(entryNode.getNode())) { destination.add('\n'); }
        stringifyNodes(entryNode.getNode(), destination, indent + 2);
      }
    } else {
      destination.add("{}\n");
    }
  }
  void stringifyArray(const Node &jNode, IDestination &destination, const unsigned long indent) const
  {
    if (!NRef<Array>(jNode).value().empty()) {
      for (const auto &jNodeNext : NRef<Array>(jNode).value()) {
        addIndentIfNewline(destination, indent);
        destination.add("- ");
        stringifyNodes(jNodeNext, destination, indent + 2);
      }
    } else {
      destination.add("[]\n");
    }
  }
  static void stringifyNumber(const Node &jNode, IDestination &destination)
  {
    destination.add(NRef<Number>(jNode).toString());
    destination.add('\n');
  }
  static void stringifyBoolean(const Node &jNode, IDestination &destination)
  {
    destination.add(NRef<Boolean>(jNode).toString());
    destination.add('\n');
  }
  static void stringifyNull(const Node &jNode, IDestination &destination)
  {
    destination.add(NRef<Null>(jNode).toString());
    destination.add('\n');
  }
  void stringifyString(const Node &jNode, IDestination &destination) const
  {
    destination.add('"');
    destination.add(yamlTranslator->to(NRef<String>(jNode).value()));
    destination.add("\"\n");
  }

  std::unique_ptr<ITranslator> yamlTranslator;
};
}// namespace JSON_Lib
