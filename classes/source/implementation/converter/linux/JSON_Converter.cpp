//
// Class: Converter
//
// Description: Convert characters to/from UTF8 and UTF16.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Error.hpp"

namespace JSON_Lib {

static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

/// <summary>
/// Convert utf8 <-> utf16 strings.
/// </summary>
std::u16string toUtf16(const std::string_view &utf8)
{
  if (utf8.find('\0') != std::string::npos) { throw Error("Tried to convert a null character."); }
  return utf16Converter.from_bytes(std::string(utf8));
}
std::string toUtf8(const std::u16string_view &utf16)
{
  if (utf16.find(u'\0') != std::string::npos) { throw Error("Tried to convert a null character."); }
  return utf16Converter.to_bytes(std::u16string(utf16));
}

}// namespace JSON_Lib
