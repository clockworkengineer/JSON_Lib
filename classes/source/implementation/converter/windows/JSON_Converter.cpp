//
// Class: Converter
//
// Description: Convert characters to/from UTF8 and UTF16.
//
// Dependencies:  Windows - character conversion API.
//

#include "Windows.h"

#include "JSON.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Error.hpp"

namespace JSON_Lib {

// ============================================================
// Windows API for converting between byte and wide characters.
// ============================================================
int WideCharToBytes(const wchar_t *wideString, const int wideStringLength, char *bytes = nullptr, const int length = 0)
{
  return WideCharToMultiByte(CP_UTF8, 0, wideString, wideStringLength, bytes, length, nullptr, nullptr);
}
int BytesToWideChar(const char *bytes, const int length, wchar_t *sideString = nullptr, const int wideStringLength = 0)
{
  return MultiByteToWideChar(CP_UTF8, 0, bytes, length, sideString, wideStringLength);
}

/// <summary>
/// Convert utf8 <-> utf16 strings.
/// </summary>
std::u16string toUtf16(const std::string_view &utf8)
{
  if (utf8.find('\0') != std::string_view::npos) { throw Error("Tried to convert a null character."); }
  std::wstring wideString(BytesToWideChar(utf8.data(), static_cast<int>(utf8.length())), 0);
  BytesToWideChar(utf8.data(), static_cast<int>(utf8.length()), &wideString[0], static_cast<int>(wideString.length()));
  return std::u16string{ wideString.begin(), wideString.end() };
}

std::string toUtf8(const std::u16string_view &utf16)
{
  if (utf16.find(u'\0') != std::string::npos) { throw Error("Tried to convert a null character."); }
  const std::wstring wideString{ utf16.begin(), utf16.end() };
  std::string bytes(WideCharToBytes(&wideString[0], static_cast<int>(wideString.length())), 0);
  WideCharToBytes(&wideString[0], -1, &bytes[0], static_cast<int>(bytes.length()));
  return bytes;
}
}// namespace JSON_Lib