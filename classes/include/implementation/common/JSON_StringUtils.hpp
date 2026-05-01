#pragma once

#include "IDestination.hpp"
#include "ITranslator.hpp"

namespace JSON_Lib {

/// @brief Append `count` space characters to destination.
inline void addIndent(IDestination &destination, const unsigned long count)
{
  for (unsigned long i = 0; i < count; ++i) { destination.add(' '); }
}

/// @brief Append a translated, double-quoted string value to destination.
inline void appendQuotedString(const std::string_view &value, IDestination &destination, const ITranslator &translator)
{
  destination.add('"');
  destination.add(translator.to(value));
  destination.add('"');
}

/// @brief Emit a trailing comma and optional newline between collection elements.
/// Decrements commaCount; emits nothing for the last element.
inline void addCommaNewline(IDestination &destination, const bool pretty, std::size_t &commaCount)
{
  if (commaCount-- > 0) {
    destination.add(',');
    if (pretty) { destination.add('\n'); }
  }
}

/// @brief Emit a closing newline and back-indented spaces after a collection.
/// No-op when not pretty-printing.
inline void addPrettyTrailer(IDestination &destination, const bool pretty,
                              const unsigned long indent, const unsigned long step)
{
  if (pretty) {
    destination.add('\n');
    addIndent(destination, indent - step);
  }
}

/// @brief Emit `indent` spaces only when the last character written was a newline.
inline void addIndentIfNewline(IDestination &destination, const unsigned long indent)
{
  if (destination.last() == '\n') { addIndent(destination, indent); }
}

}// namespace JSON_Lib
