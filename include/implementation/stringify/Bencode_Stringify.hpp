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

  void stringify(const JNode &jNode, IDestination &destination, long indent) const override;
  [[nodiscard]] long getIndent() const override;
  void setIndent(long indent) override;

private:
  // Stringify Bencode
  static void stringifyNumber(const JNode &jNode, IDestination &destination);
  static void stringifyString(const JNode &jNode, IDestination &destination);
  static void stringifyBoolean(const JNode &jNode, IDestination &destination);
  static void stringifyNull(const JNode &jNode, IDestination &destination);
  static void stringifyHole(const JNode &jNode, IDestination &destination);
  void stringifyObject(const JNode &jNode, IDestination &destination, long indent) const;
  void stringifyArray(const JNode &jNode, IDestination &destination, long indent) const;

  Bencode_Translator bencodeTranslator;
};


}// namespace JSON_Lib
