#pragma once

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"

namespace JSON_Lib {

class Bencode_Stringify : public IStringify
{
public:
  Bencode_Stringify() = default;
  Bencode_Stringify(const Bencode_Stringify &other) = delete;
  Bencode_Stringify &operator=(const Bencode_Stringify &other) = delete;
  Bencode_Stringify(Bencode_Stringify &&other) = delete;
  Bencode_Stringify &operator=(Bencode_Stringify &&other) = delete;
  ~Bencode_Stringify() = default;

  virtual void stringify(const JNode &jNode, IDestination &destination, long indent) const override;
  virtual long getIndent() const override;
  virtual void setIndent(long indent) override;

private:
  // Stringify Bencode
  void stringifyNumber(const JNode &jNode, IDestination &destination) const;
  void stringifyString(const JNode &jNode, IDestination &destination) const;
  void stringifyBoolean(const JNode &jNode, IDestination &destination) const;
  void stringifyNull(const JNode &jNode, IDestination &destination) const;
  void stringifyHole(const JNode &jNode, IDestination &destination) const;
  void stringifyObject(const JNode &jNode, IDestination &destination, long indent) const;
  void stringifyArray(const JNode &jNode, IDestination &destination, long indent) const;
};

}// namespace JSON_Lib
