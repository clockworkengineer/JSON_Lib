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
  void stringify(const JNode &jNode, IDestination &destination, const long indent) const override
  {
    if (jNode.isNumber()) {
      destination.add("i" + std::to_string(JRef<Number>(jNode).value<int>()) + "e");
    } else if (jNode.isString()) {
      destination.add(
        std::to_string(static_cast<int>(JRef<String>(jNode).value().length())) + ":" + JRef<String>(jNode).value());
    } else if (jNode.isBoolean()) {
      if (JRef<Boolean>(jNode).value()) {
        destination.add("4:True");
      } else {
        destination.add("5:False");
      }
    } else if (jNode.isNull()) {
      destination.add("4:null");
    } else if (jNode.isHole()) {
      ;
    } else if (jNode.isObject()) {
      destination.add('d');
      for (auto &entry : JRef<Object>(jNode).value()) {
        destination.add(
          std::to_string(static_cast<int>(JRef<String>(entry.getKey()).value().length())) + ":" + JRef<String>(entry.getKey()).value());
        stringify(entry.getJNode(), destination, 0);
      }
      destination.add("e");
    } else if (jNode.isArray()) {
      destination.add('l');
      if (!JRef<Array>(jNode).value().empty()) {
        for (auto &entry : JRef<Array>(jNode).value()) { stringify(entry, destination, 0); }
      }
      destination.add("e");
    } else {
      throw Error("Unknown JNode type encountered during stringification.");
    }
  }

private:

  Bencode_Translator bencodeTranslator;
};


}// namespace JSON_Lib
