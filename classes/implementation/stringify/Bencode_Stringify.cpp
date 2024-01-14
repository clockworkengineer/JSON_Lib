//
// Class: Bencode_Stringify
//
// Description:
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"

#include "Bencode_Stringify.hpp"

namespace JSON_Lib {

/// <summary>
/// Convert Number JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Number JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void Bencode_Stringify::stringifyNumber(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert String JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void Bencode_Stringify::stringifyString(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert Boolean JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void Bencode_Stringify::stringifyBoolean(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert Null JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void Bencode_Stringify::stringifyNull(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert Hole JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void Bencode_Stringify::stringifyHole(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert Object JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringifyObject(const JNode &jNode, IDestination &destination, long indent) const {}

/// <summary>
/// Convert Array JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringifyArray(const JNode &jNode, IDestination &destination, long indent) const {}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void Bencode_Stringify::stringify(const JNode &jNode, IDestination &destination, long indent) const
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
    throw JSON::Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSON_Lib
