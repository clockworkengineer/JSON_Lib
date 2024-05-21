//
// Class: Bencode_Stringify
//
// Description: Stringify JNode tree to Bencode on destination stream.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "Bencode_Stringify.hpp"

namespace JSON_Lib {

/// <summary>
/// Convert Number JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Number JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
void Bencode_Stringify::stringifyNumber(const JNode &jNode, IDestination &destination)
{
  // All numbers to rounded integers
  destination.add("i" + std::to_string(JRef<Number>(jNode).value<int>()) + "e");
}

/// <summary>
/// Convert String JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
void Bencode_Stringify::stringifyString(const JNode &jNode, IDestination &destination)
{
  destination.add(
    std::to_string(static_cast<int>(JRef<String>(jNode).value().length())) + ":" + JRef<String>(jNode).value());
}

/// <summary>
/// Convert Boolean JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
void Bencode_Stringify::stringifyBoolean(const JNode &jNode, IDestination &destination)
{
  if (JRef<Boolean>(jNode).value()) {
    destination.add("4:True");
  } else {
    destination.add("5:False");
  }
}

/// <summary>
/// Convert Null JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
void Bencode_Stringify::stringifyNull([[maybe_unused]] const JNode &jNode, IDestination &destination) const
{
  destination.add("4:null");
}

/// <summary>
/// Convert Hole JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
void Bencode_Stringify::stringifyHole([[maybe_unused]] const JNode &jNode,
  [[maybe_unused]] IDestination &destination) const
{}

/// <summary>
/// Convert Object JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringifyObject(const JNode &jNode,
  IDestination &destination,
  [[maybe_unused]] long indent) const
{
  destination.add('d');
  for (auto &entry : JRef<Object>(jNode).value()) {
    stringifyString(entry.getKey(), destination);
    stringify(entry.getJNode(), destination, 0);
  }
  destination.add("e");
}

/// <summary>
/// Convert Array JNode to Bencode on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for Bencode.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringifyArray(const JNode &jNode,
  IDestination &destination,
  [[maybe_unused]] long indent) const
{
  destination.add('l');
  if (!JRef<Array>(jNode).value().empty()) {
    for (auto &entry : JRef<Array>(jNode).value()) { stringify(entry, destination, 0); }
  }
  destination.add("e");
}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringify(const JNode &jNode, IDestination &destination, const long indent) const
{
  if (jNode.isNumber()) {
    stringifyNumber(jNode, destination);
  } else if (jNode.isString()) {
    stringifyString(jNode, destination);
  } else if (jNode.isBoolean()) {
    stringifyBoolean(jNode, destination);
  } else if (jNode.isNull()) {
    stringifyNull(jNode, destination);
  } else if (jNode.isHole()) {
    stringifyHole(jNode, destination);
  } else if (jNode.isObject()) {
    stringifyObject(jNode, destination, indent);
  } else if (jNode.isArray()) {
    stringifyArray(jNode, destination, indent);
  } else {
    throw Error("Unknown JNode type encountered during stringification.");
  }
}

long Bencode_Stringify::getIndent() const { return 0; }

void Bencode_Stringify::setIndent([[maybe_unused]] long indent) {}

}// namespace JSON_Lib
