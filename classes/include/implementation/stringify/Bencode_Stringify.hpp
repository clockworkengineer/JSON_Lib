#pragma once

#include <memory>
#include "JSON.hpp"
#include "JSON_Core.hpp"

namespace JSON_Lib {

class Bencode_Stringify final : public IStringify
{
public:
  explicit Bencode_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
      : bencodeTranslator(std::move(translator))
  {
  }
  Bencode_Stringify(const Bencode_Stringify &other) = delete;
  Bencode_Stringify &operator=(const Bencode_Stringify &other) = delete;
  Bencode_Stringify(Bencode_Stringify &&other) = delete;
  Bencode_Stringify &operator=(Bencode_Stringify &&other) = delete;
  ~Bencode_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified Bencode.</param>
  /// <param name="indent">Current print indentation.</param>
  void stringify(const Node &jNode, IDestination &destination, const unsigned long indent) const override
  {
    stringifyNodes(jNode, destination, indent);
  }

private:
  static void stringifyNodes(const Node &jNode, IDestination &destination, const unsigned long)
  {
    jNode.visit(overloaded{
      [&](const Number &) { stringifyNumber(jNode, destination); },
      [&](const String &) { stringifyString(jNode, destination); },
      [&](const Boolean &) { stringifyBoolean(jNode, destination); },
      [&](const Null &) { stringifyNull(jNode, destination); },
      [&](const Hole &) {},
      [&](const Object &) { stringifyObject(jNode, destination, 0); },
      [&](const Array &) { stringifyArray(jNode, destination, 0); },
      [&](const std::monostate &) { throw Error("Unknown Node type encountered during stringification."); }
    });
  }
  static void stringifyObject(const Node &jNode, IDestination &destination, const unsigned long)
  {
    destination.add('d');
    for (auto &entry : NRef<Object>(jNode).value()) {
      stringifyString(entry.getKey(), destination);
      stringifyNodes(entry.getNode(), destination, 0);
    }
    destination.add("e");
  }
  static void stringifyArray(const Node &jNode, IDestination &destination, const unsigned long)
  {
    destination.add('l');
    for (auto &entry : NRef<Array>(jNode).value()) { stringifyNodes(entry, destination, 0); }
    destination.add("e");
  }
  static void stringifyNumber(const Node &jNode, IDestination &destination)
  {
    destination.add('i');
    destination.add(std::to_string(NRef<Number>(jNode).value<long long>()));
    destination.add('e');
  }
  static void stringifyBoolean(const Node &jNode, IDestination &destination)
  {
    if (NRef<Boolean>(jNode).value()) {
      destination.add("4:True");
    } else {
      destination.add("5:False");
    }
  }
  static void stringifyNull(const Node &, IDestination &destination) { destination.add("4:null"); }
  static void stringifyString(const Node &jNode, IDestination &destination)
  {
    stringifyString(NRef<String>(jNode).value(), destination);
  }
  static void stringifyString(const std::string_view &value, IDestination &destination)
  {
    destination.add(std::to_string(static_cast<int>(value.length())));
    destination.add(':');
    destination.add(value);
  }

  std::unique_ptr<ITranslator> bencodeTranslator;
};


}// namespace JSON_Lib
