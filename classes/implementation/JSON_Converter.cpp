//
// Class: JSON_Converter
//
// Description: Convert characters to/from UTF8 and UTF16.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Converter.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
// NAMESPACE
// =========
namespace JSONLib
{
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
        /// Windows API for converting between byte and wide characters.
        /// </summary>
#if defined(_WIN64)
        int WideCharToBytes(wchar_t *sideString, int wideStringLength, char *bytes = NULL, int length = 0)
        {
                return (WideCharToMultiByte(CP_UTF8, 0, sideString, wideStringLength, bytes, length, NULL, NULL));
        }
        int BytesToWideChar(const char *bytes, int length, wchar_t *sideString = NULL, int wideStringLength = 0)
        {
                return (MultiByteToWideChar(CP_UTF8, 0, bytes, length, sideString, wideStringLength));
        }
#endif
        // ==============
        // PUBLIC METHODS
        // ==============
        /// <summary>
        /// Convert utf8 <-> utf16 strings.
        /// </summary>
        std::u16string JSON_Converter::to_utf16(const std::string &utf8)
        {
#if defined(_WIN64)
                std::wstring wideString(BytesToWideChar(utf8.c_str(), static_cast<int>(utf8.length())), 0);
                BytesToWideChar(utf8.c_str(), static_cast<int>(utf8.length()), &wideString[0], static_cast<int>(wideString.length()));
                return (std::u16string{wideString.begin(), wideString.end()});
#else
                return (m_UTF16.from_bytes(utf8));
#endif
        }
        std::string JSON_Converter::to_utf8(const std::u16string &utf16)
        {
#if defined(_WIN64)
                std::wstring wideString{utf16.begin(), utf16.end()};
                std::string bytes(WideCharToBytes(&wideString[0], static_cast<int>(wideString.length())), 0);
                WideCharToBytes(&wideString[0], -1, &bytes[0], static_cast<int>(bytes.length()));
                return bytes;
#else
                return (m_UTF16.to_bytes(utf16));
#endif
        }
} // namespace JSONLib
