//
// Class: Converter
//
// Description: Convert characters to/from UTF8 and UTF16.
//
// Dependencies:   C20++ - Language standard features used.
//
// ==========================
// C++ STL
// ==========================
#include <codecvt>
#include <locale>
// =================
// CLASS DEFINITIONS
// =================
#include "JSON_Converter.hpp"
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
static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> m_UTF16;
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// ===============
// PRIVATE METHODS
// ===============
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
/// Convert utf8 <-> utf16 strings.
/// </summary>
std::u16string JSON_Converter::toUtf16(const std::string &utf8) const
{
  return (m_UTF16.from_bytes(utf8));
}
std::string JSON_Converter::toUtf8(const std::u16string &utf16) const
{
  return (m_UTF16.to_bytes(utf16));
}
}// namespace JSONLib
