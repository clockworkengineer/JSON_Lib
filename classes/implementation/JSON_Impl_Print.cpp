//
// Class: JSON_Impl_Print
//
// Description: JSON class implementation layer that uses recursion to reconstitute
// the JSON tree presentation back into raw JSON string adding suitable idents where
// necessary (pretty print).
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
void JSON_Impl::printNumber(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Number>(jNode).toString());
}
/// <summary>
/// Convert String JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">String JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::printString(const JNode &jNode, IDestination &destination)
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
void JSON_Impl::printBoolean(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Boolean>(jNode).toString());
}
/// <summary>
/// Convert Null JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Null JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::printNull(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Null>(jNode).toString());
}
/// <summary>
/// Convert Hole JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Hole JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
void JSON_Impl::printHole(const JNode &jNode, IDestination &destination)
{
  destination.add(JRef<Hole>(jNode).toString());
}
/// <summary>
/// Convert Object JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Object JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Impl::printObject(const JNode &jNode, IDestination &destination, long indent)
{
  size_t commaCount = JRef<Object>(jNode).getObjectEntries().size() - 1;
  destination.add("{\n");
  for (auto &entry : JRef<Object>(jNode).getObjectEntries()) {
    destination.add(std::string(indent, ' '));
    destination.add('"');
    destination.add(m_translator->toJSON(entry.getKey()));
    destination.add("\": ");
    printJNodes(entry.getJNode(), destination, indent + m_indent);
    if (commaCount-- > 0) { destination.add(",\n"); };
  }
  destination.add("\n");
  destination.add(std::string(indent - m_indent, ' '));
  destination.add("}");
}
/// <summary>
/// Convert Array JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Impl::printArray(const JNode &jNode, IDestination &destination, long indent)
{
  destination.add('[');
  if (!JRef<Array>(jNode).getArrayEntries().empty()) {
    size_t commaCount = JRef<Array>(jNode).getArrayEntries().size() - 1;
    destination.add('\n');
    for (auto &node : JRef<Array>(jNode).getArrayEntries()) {
      destination.add(std::string(indent, ' '));
      printJNodes(node, destination, indent + m_indent);
      if (commaCount-- > 0) { destination.add(",\n"); };
    }
    destination.add('\n');
    destination.add(std::string(indent - m_indent, ' '));
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
void JSON_Impl::printJNodes(const JNode &jNode, IDestination &destination, long indent)
{
  switch (jNode.getVariant().getType()) {
  case JNode::Type::number:
    printNumber(jNode, destination);
    break;
  case JNode::Type::string:
    printString(jNode, destination);
    break;
  case JNode::Type::boolean:
    printBoolean(jNode, destination);
    break;
  case JNode::Type::null:
    printNull(jNode, destination);
    break;
  case JNode::Type::hole:
    printHole(jNode, destination);
    break;
  case JNode::Type::object:
    printObject(jNode, destination, indent);
    break;
  case JNode::Type::array:
    printArray(jNode, destination, indent);
    break;
  default:
    throw Error("Unknown JNode type encountered during print.");
  }
}
}// namespace JSONLib
