//
// Class: JSON_Impl
//
// Description: JSON class implementation layer to strip all whitespace from
// JSON text source stream and output on a destination stream.
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
/// Strip all whitespace from a JSON source.
/// </summary>
/// <param name="source">Source of JSON.</param>
/// <param name="destination">Destination for stripped JSON.</param>
void JSON_Impl::stripWhitespace(ISource &source, IDestination &destination)
{
  while (source.more()) {
    if (!source.isWS()) {
      destination.add(source.current());
      if (source.current() == '"') {
        destination.add(extractString(source, false));
        destination.add('"');
        continue;
      }
    }
    source.next();
  }
}
}// namespace JSONLib
