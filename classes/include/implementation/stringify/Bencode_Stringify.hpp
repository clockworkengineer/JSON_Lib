#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Default_Translator.hpp"

namespace JSON_Lib {

class Bencode_Stringify final : public IStringify
{
public:
  explicit Bencode_Stringify(std::unique_ptr<ITranslator> translator = std::make_unique<Default_Translator>())
    : bencodeTranslator(std::move(translator)) {}
  Bencode_Stringify &operator=(const Bencode_Stringify &other) = delete;
  Bencode_Stringify(Bencode_Stringify &&other) = delete;
  Bencode_Stringify &operator=(Bencode_Stringify &&other) = delete;
  ~Bencode_Stringify() override = default;

  /// <summary>
  /// Recursively traverse JNode structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="jNode">JNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified Bencode.</param>
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
    } else if (isA<Object>(jNode)) {
      stringifyObject(jNode, destination, 0);
    } else if (isA<Array>(jNode)) {
      stringifyArray(jNode, destination, 0);
    } else {
      throw Error("Unknown JNode type encountered during stringification.");
    }
  }

private:
  void stringifyObject(const JNode &jNode, IDestination &destination, [[maybe_unused]] const unsigned long indent) const
  {
    destination.add('d');
    for (auto &entry : JRef<Object>(jNode).value()) {
      stringify(entry.getKeyJNode(), destination, 0);
      stringify(entry.getJNode(), destination, 0);
    }
    destination.add("e");
  }
  void stringifyArray(const JNode &jNode, IDestination &destination, [[maybe_unused]]const unsigned long indent) const
  {
    destination.add('l');
    for (auto &entry : JRef<Array>(jNode).value()) { stringify(entry, destination, 0); }
    destination.add("e");
  }
  static void stringifyNumber(const JNode &jNode, IDestination &destination)
  {
    destination.add("i" + std::to_string(JRef<Number>(jNode).value<long long>()) + "e");
  }
  static void stringifyBoolean(const JNode &jNode, IDestination &destination)
  {
    if (JRef<Boolean>(jNode).value()) {
      destination.add("4:True");
    } else {
      destination.add("5:False");
    }
  }
  static void stringifyNull([[maybe_unused]]const JNode &jNode, IDestination &destination) { destination.add("4:null"); }
  static void stringifyString(const JNode &jNode, IDestination &destination)
  {
    const auto jsonString = JRef<String>(jNode).value();
    destination.add(std::to_string(static_cast<int>(jsonString.length())) + ":" + jsonString);
  }

  std::unique_ptr<ITranslator> bencodeTranslator;
};


}// namespace JSON_Lib
