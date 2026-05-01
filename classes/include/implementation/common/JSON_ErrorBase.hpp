#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace JSON_Lib {

// ================================================================
// Shared helper for the nested Error structs throughout the library.
// Produces a string of the form "<tag> Error: <msg>".
// ================================================================
inline std::string makeTaggedError(const std::string_view tag, const std::string_view msg)
{
  std::string s;
  s.reserve(tag.size() + 8 + msg.size()); // " Error: " = 8 chars
  s.append(tag).append(" Error: ").append(msg);
  return s;
}

} // namespace JSON_Lib
