//
// Class: XML_Stringify
//
// Description: Stringify JNode tree to XML on destination stream.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"

#include "XML_Stringify.hpp"

namespace JSON_Lib {

/// <summary>
/// Convert Number JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Number JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyNumber(const JNode &jNode, IDestination &destination) const
{
  // All numbers to rounded integers
  destination.add("i" + std::to_string(JRef<Number>(jNode).value<int>()) + "e");
}

/// <summary>
/// Convert String JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyString(const JNode &jNode, IDestination &destination) const
{
  destination.add(JRef<String>(jNode).value());
}

/// <summary>
/// Convert Boolean JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyBoolean(const JNode &jNode, IDestination &destination) const
{
  // if (JRef<Boolean>(jNode).value()) {
  //   destination.add("4:True");
  // } else {
  //   destination.add("5:False");
  // }
}

/// <summary>
/// Convert Null JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyNull(const JNode &jNode, IDestination &destination) const { destination.add("4:null"); }

/// <summary>
/// Convert Hole JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyHole(const JNode &jNode, IDestination &destination) const {}

/// <summary>
/// Convert Object JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyObject(const JNode &jNode, IDestination &destination, long indent) const
{
  // destination.add('d');
  // for (auto &entry : JRef<Object>(jNode).value()) {
  //   stringifyString(entry.getKey(), destination);
  //   stringifyXML(entry.getJNode(), destination, 0);
  // }
  // destination.add("e");
}

/// <summary>
/// Convert Array JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyArray(const JNode &jNode, IDestination &destination, long indent) const
{
  // destination.add('l');
  // if (!JRef<Array>(jNode).value().empty()) {
  //   for (auto &entry : JRef<Array>(jNode).value()) { stringifyXML(entry, destination, 0); }
  // }
  // destination.add("e");
}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyXML(const JNode &jNode, IDestination &destination, long indent) const
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

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringify(const JNode &jNode, IDestination &destination, long indent) const
{
  destination.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  destination.add("<root>");
  stringifyXML(jNode, destination, 0);
  destination.add("</root>");
}

long XML_Stringify::getIndent() const { return (0); }

void XML_Stringify::setIndent(long indent) {}

}// namespace JSON_Lib
