//
// Class: JSON_Impl
//
// Description: JSON class implementation layer that uses recursion to reconstitute
// the JSON tree presentation back into raw JSON string without any whitespace
// (stringification) or with indentation and whitespace pretty print.
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
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
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
// =================
// PRIVATE FUNCTIONS
// =================
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
/// <param name="indent">Current print indentation.</param>
void JSON_Impl::stringifyObject(const JNode &jNode, IDestination &destination, long indent)
{
  size_t commaCount = JRef<Object>(jNode).getObjectEntries().size() - 1;
  destination.add('{');
  if (indent != 0) { destination.add('\n'); };
  for (auto &entry : JRef<Object>(jNode).getObjectEntries()) {
    if (indent != 0) { destination.add(std::string(indent, ' ')); };
    stringifyString(entry.getKey(), destination);
    destination.add(":");
    if (indent != 0) { destination.add(" "); }
    stringifyJNodes(entry.getJNode(), destination, (indent != 0) ? (indent + m_indent) : 0);
    if (commaCount-- > 0) {
      destination.add(",");
      if (indent != 0) { destination.add('\n'); }
    }
  }
  if (indent != 0) {
    destination.add('\n');
    destination.add(std::string(indent - m_indent, ' '));
  }
  destination.add("}");
}
/// <summary>
/// Convert Array JNode to JSON on destination stream.
/// </summary>
/// <param name="jNode">Array JNode.</param>
/// <param name="destination">Destination stream for JSON.</param>
/// <param name="indent">Current print indentation.</param>
void JSON_Impl::stringifyArray(const JNode &jNode, IDestination &destination, long indent)
{
  destination.add('[');
  if (!JRef<Array>(jNode).getArrayEntries().empty()) {
    size_t commaCount = JRef<Array>(jNode).getArrayEntries().size() - 1;
    if (indent != 0) { destination.add('\n'); };
    for (auto &entry : JRef<Array>(jNode).getArrayEntries()) {
      if (indent != 0) { destination.add(std::string(indent, ' ')); };
      stringifyJNodes(entry, destination, (indent != 0) ? (indent + m_indent) : 0);
      if (commaCount-- > 0) {
        destination.add(",");
        if (indent != 0) { destination.add('\n'); }
      }
    }
    if (indent != 0) {
      destination.add('\n');
      destination.add(std::string(indent - m_indent, ' '));
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
void JSON_Impl::stringifyJNodes(const JNode &jNode, IDestination &destination, long indent)
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
    throw Error("Unknown JNode type encountered during stringification.");
  }
}
}// namespace JSON_Lib
