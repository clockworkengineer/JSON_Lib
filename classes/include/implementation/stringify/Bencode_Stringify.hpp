#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Bencode_Translator.hpp"

namespace JSON_Lib {

class Bencode_Stringify final : public IStringify
{
public:
  Bencode_Stringify() = default;
  Bencode_Stringify &operator=(const Bencode_Stringify &other) = delete;
  Bencode_Stringify(Bencode_Stringify &&other) = delete;
  Bencode_Stringify &operator=(Bencode_Stringify &&other) = delete;
  ~Bencode_Stringify() override = default;

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
      destination.add("i" + std::to_string(JRef<Number>(jNode).value<long long>()) + "e");
    } else if (isA<String>(jNode)) {
      const auto jsonString = JRef<String>(jNode).value();
      destination.add(std::to_string(static_cast<int>(jsonString.length())) + ":" + jsonString);
    } else if (isA<Boolean>(jNode)) {
      if (JRef<Boolean>(jNode).value()) {
        destination.add("4:True");
      } else {
        destination.add("5:False");
      }
    } else if (isA<Null>(jNode)) {
      destination.add("4:null");
    } else if (isA<Hole>(jNode)) {
    } else if (isA<Object>(jNode)) {
      destination.add('d');
      for (auto &entry : JRef<Object>(jNode).value()) {
        stringify(entry.getKeyJNode(), destination, 0);
        stringify(entry.getJNode(), destination, 0);
      }
      destination.add("e");
    } else if (isA<Array>(jNode)) {
      destination.add('l');
      for (auto &entry : JRef<Array>(jNode).value()) { stringify(entry, destination, 0); }
      destination.add("e");
    } else {
      throw Error("Unknown JNode type encountered during stringification.");
    }
  }

private:

  Bencode_Translator bencodeTranslator;
};


}// namespace JSON_Lib
