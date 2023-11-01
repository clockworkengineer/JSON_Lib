//
// Class: Converter
//
// Description: Convert characters to/from UTF8 and UTF16.
//
// Dependencies: C++20 - Language standard features used.
//

#include <codecvt>
#include <locale>

#include "JSON_Converter.hpp"

namespace JSON_Lib {

static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

/// <summary>
/// Convert utf8 <-> utf16 strings.
/// </summary>
std::u16string JSON_Converter::toUtf16(const std::string &utf8) const { return (utf16Converter.from_bytes(utf8)); }
std::string JSON_Converter::toUtf8(const std::u16string &utf16) const { return (utf16Converter.to_bytes(utf16)); }

}// namespace JSON_Lib
