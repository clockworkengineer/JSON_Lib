//
// Class: JSON_Stringify
//
// Description:
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Core.hpp"

#include "JSON_Stringify.hpp"

namespace JSON_Lib {

/// <summary>
/// Convert Number JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Number JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Stringify::stringifyNumber(const JNode &jNode, IDestination &destination) const
{
  destination.add(JRef<Number>(jNode).toString());
}

/// <summary>
/// Convert String JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Stringify::stringifyString(const JNode &jNode, IDestination &destination) const
{
  destination.add('"');
  destination.add(jsonTranslator.toJSON(JRef<String>(jNode).toString()));
  destination.add('"');
}

/// <summary>
/// Convert Boolean JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Stringify::stringifyBoolean(const JNode &jNode, IDestination &destination) const
{
  destination.add(JRef<Boolean>(jNode).toString());
}

/// <summary>
/// Convert Null JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Stringify::stringifyNull(const JNode &jNode, IDestination &destination) const
{
  destination.add(JRef<Null>(jNode).toString());
}

/// <summary>
/// Convert Hole JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Stringify::stringifyHole(const JNode &jNode, IDestination &destination) const
{
  destination.add(JRef<Hole>(jNode).toString());
}

/// <summary>
/// Convert Object JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Stringify::stringifyObject(const JNode &jNode, IDestination &destination, long indent) const
{
  size_t commaCount = JRef<Object>(jNode).value().size() - 1;
  destination.add('{');
  if (indent != 0) { destination.add('\n'); };
  for (auto &entry : JRef<Object>(jNode).value()) {
    if (indent != 0) { destination.add(std::string(indent, ' ')); };
    stringifyString(entry.getKey(), destination);
    destination.add(":");
    if (indent != 0) { destination.add(" "); }
    stringify(entry.getJNode(), destination, (indent != 0) ? (indent + printIndent) : 0);
    if (commaCount-- > 0) {
      destination.add(",");
      if (indent != 0) { destination.add('\n'); }
    }
  }
  if (indent != 0) {
    destination.add('\n');
    destination.add(std::string(indent - printIndent, ' '));
  }
  destination.add("}");
}

/// <summary>
/// Convert Array JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Stringify::stringifyArray(const JNode &jNode, IDestination &destination, long indent) const
{
  destination.add('[');
  if (!JRef<Array>(jNode).getArrayEntries().empty()) {
    size_t commaCount = JRef<Array>(jNode).getArrayEntries().size() - 1;
    if (indent != 0) { destination.add('\n'); };
    for (auto &entry : JRef<Array>(jNode).getArrayEntries()) {
      if (indent != 0) { destination.add(std::string(indent, ' ')); };
      stringify(entry, destination, (indent != 0) ? (indent + printIndent) : 0);
      if (commaCount-- > 0) {
        destination.add(",");
        if (indent != 0) { destination.add('\n'); }
      }
    }
    if (indent != 0) {
      destination.add('\n');
      destination.add(std::string(indent - printIndent, ' '));
    }
  }
  destination.add("]");
}

/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Stringify::stringify(const JNode &jNode, IDestination &destination, long indent) const
{
  switch (jNode.getVariant().getType()) {
  case Variant::Type::number:
    stringifyNumber(jNode, destination);
    break;
  case Variant::Type::string:
    stringifyString(jNode, destination);
    break;
  case Variant::Type::boolean:
    stringifyBoolean(jNode, destination);
    break;
  case Variant::Type::null:
    stringifyNull(jNode, destination);
    break;
  case Variant::Type::hole:
    stringifyHole(jNode, destination);
    break;
  case Variant::Type::object:
    stringifyObject(jNode, destination, indent);
    break;
  case Variant::Type::array:
    stringifyArray(jNode, destination, indent);
    break;
  default:
    throw JSON::Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSON_Lib
