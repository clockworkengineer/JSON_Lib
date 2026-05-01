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

}// namespace JSON_Lib
