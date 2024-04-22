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
  destination.add(std::to_string(JRef<Number>(jNode).value<int>()));
}

/// <summary>
/// Convert String JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyString(const JNode &jNode, IDestination &destination) const
{
  destination.add(xmlTranslator.to(JRef<String>(jNode).value()));
}

/// <summary>
/// Convert Boolean JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyBoolean(const JNode &jNode, IDestination &destination) const
{
  if (JRef<Boolean>(jNode).value()) {
    destination.add("True");
  } else {
    destination.add("False");
  }
}

/// <summary>
/// Convert Null JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyNull([[maybe_unused]] const JNode &jNode, [[maybe_unused]] IDestination &destination) const
{}

/// <summary>
/// Convert Hole JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
void XML_Stringify::stringifyHole([[maybe_unused]] const JNode &jNode, [[maybe_unused]] IDestination &destination) const
{}

/// <summary>
/// Convert Object JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyObject(const JNode &jNode, IDestination &destination, [[maybe_unused]] long indent) const
{
  for (const auto &jNodeNext : JRef<Object>(jNode).value()) {
    auto elementName = JRef<String>(jNodeNext.getKey()).value();
    std::replace(elementName.begin(), elementName.end(), ' ', '-');
    destination.add("<" + elementName + ">");
    stringifyXML(jNodeNext.getJNode(), destination, 0);
    destination.add("</" + elementName + ">");
  }
}

/// <summary>
/// Convert Array JNode to XML on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for XML.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringifyArray(const JNode &jNode, IDestination &destination, [[maybe_unused]] long indent) const
{
  if (JRef<Array>(jNode).value().size() > 1) {
    for (const auto &bNodeNext : JRef<Array>(jNode).value()) {
      destination.add("<Row>");
      stringifyXML(bNodeNext, destination, 0);
      destination.add("</Row>");
    }
  }
}

/// <summary>
/// Recursively traverse JNode structure encoding it into XML string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified XML.</param>
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
    throw Error("Unknown JNode type encountered during stringification.");
  }
}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void XML_Stringify::stringify(const JNode &jNode, IDestination &destination, [[maybe_unused]] long indent) const
{
  destination.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  destination.add("<root>");
  stringifyXML(jNode, destination, 0);
  destination.add("</root>");
}

long XML_Stringify::getIndent() const { return (0); }

void XML_Stringify::setIndent([[maybe_unused]] long indent) {}

}// namespace JSON_Lib
