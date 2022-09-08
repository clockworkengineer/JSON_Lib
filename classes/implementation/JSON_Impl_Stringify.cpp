//
// Class: JSON_Impl
//
// Description: JSON class implementation layer that uses recursion to reconstitute
// the JSON tree presentation back into raw JSON string without any whitespace
// (stringification).
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Impl.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ===========================
// PRIVATE TYPES AND CONSTANTS
// ===========================
// ==========================
// PUBLIC TYPES AND CONSTANTS
// ==========================
// ========================
// PRIVATE STATIC VARIABLES
// ========================
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// ===============
// PRIVATE METHODS
// ===============
/// <summary>
/// Convert Number JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Number JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyNumber(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Number>(jNode).toString());
}
/// <summary>
/// Convert String JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyString(const JNode &jNode, IDestination &destination)
{
  destination.add('"');
  destination.add(m_translator->toJSON(JRef<String>(jNode).toString()));
  destination.add('"');
}
/// <summary>
/// Convert Boolean JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Boolean JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyBoolean(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Boolean>(jNode).toString());
}
/// <summary>
/// Convert Null JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyNull(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Null>(jNode).toString());
}
/// <summary>
/// Convert Hole JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyHole(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Hole>(jNode).toString());
}
/// <summary>
/// Convert Object JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyObject(const JNode &jNode, IDestination &destination)
{
  size_t commaCount = JRef<Object>(jNode).getObjectEntries().size() - 1;
  destination.add('{');
  for (auto &entry : JRef<Object>(jNode).getObjectEntries()) {
    stringifyString(entry.getKey(), destination);
    destination.add(":");
    stringifyJNodes(entry.getJNode(), destination);
    if (commaCount-- > 0) { destination.add(','); };
  }
  destination.add('}');
}
/// <summary>
/// Convert Array JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::stringifyArray(const JNode &jNode, IDestination &destination)
{
  size_t commaCount = JRef<Array>(jNode).getArrayEntries().size() - 1;
  destination.add('[');
  for (auto &node : JRef<Array>(jNode).getArrayEntries()) {
    stringifyJNodes(node, destination);
    if (commaCount-- > 0) { destination.add(','); };
  }
  destination.add(']');
}
/// <summary>
/// Recursively traverse JNode structure encoding it into JSON string on
/// the destination stream passed in.
/// </summary>
/// <param name=jNode>JNode structure to be traversed.</param>
/// <param name=destination>Destination stream for stringified JSON.</param>
void JSON_Impl::stringifyJNodes(const JNode &jNode, IDestination &destination)
{
  switch (jNode.getVariant().getType()) {
  case JNode::Type::number:
    stringifyNumber(jNode, destination);
    break;
  case JNode::Type::string:
    stringifyString(jNode, destination);
    break;
  case JNode::Type::boolean:
    stringifyBoolean(jNode, destination);
    break;
  case JNode::Type::null:
    stringifyNull(jNode, destination);
    break;
  case JNode::Type::hole:
    stringifyHole(jNode, destination);
    break;
  case JNode::Type::object:
    stringifyObject(jNode, destination);
    break;
  case JNode::Type::array:
    stringifyArray(jNode, destination);
    break;
  default:
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSONLib
