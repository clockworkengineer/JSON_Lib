//
// Class: JSON_Translator
//
// Description: Translate to/from JSON string escapes within source
// strings. This is the default translator but is possible to write
// a custom one and pass it to the JSON class constructor to be used.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Translator.hpp"
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
#if defined(_WIN64)
#include "Windows.h"
#else
    static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> m_UTF16;
#endif
    // =======================
    // PUBLIC STATIC VARIABLES
    // =======================
    // ===============
    // PRIVATE METHODS
    // ===============
    /// <summary>
    /// Convert utf8 <-> utf16 strings.
    /// </summary>
    static std::u16string utf8_to_utf16(const std::string &utf8)
    {
#if defined(_WIN64)
        int count = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.length()), NULL, 0);
        std::wstring wstr(count, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.length()), &wstr[0], count);
        return (std::u16string{wstr.begin(), wstr.end()});
#else
        return(m_UTF16.from_bytes(utf8));
#endif
    }
    static std::string utf16_to_utf8(const std::u16string &utf16)
    {
#if defined(_WIN64)
        std::wstring wstr{utf16.begin(), utf16.end()};
        int count = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
        std::string str(count, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
        return str;
#else
        return (m_UTF16.to_bytes(utf16));
#endif
    }
    /// <summary>
    /// Initialise tables used to convert to/from single character
    /// escape sequences within a JSON string.
    /// </summary>
    void JSON_Translator::initialiseTranslationMaps()
    {
        // From Escape sequence
        m_from['\\'] = u"\\";
        m_from['t'] = u"\t";
        m_from['\"'] = u"\"";
        m_from['/'] = u"/";
        m_from['b'] = u"\b";
        m_from['f'] = u"\f";
        m_from['n'] = u"\n";
        m_from['r'] = u"\r";
        // To Escape sequence
        m_to['\\'] = "\\\\";
        m_to['\t'] = "\\t";
        m_to['"'] = "\\\"";
        m_to['/'] = "\\/";
        m_to['\b'] = "\\b";
        m_to['\f'] = "\\f";
        m_to['\n'] = "\\n";
        m_to['\r'] = "\\r";
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// Convert any escape sequences in a string to their correct sequence
    //  of UTF-8 characters. If input string contains any unpaired surrogates
    //  then this is deemed as a syntax error and an error is duely thrown.
    /// </summary>
    /// <param name="jsonString">JSON string to process.</param>
    /// <returns>String with escapes translated.</returns>
    std::string JSON_Translator::from(const std::string &jsonString)
    {
        std::u16string workBuffer;
        auto current = jsonString.begin();
        while (current != jsonString.end())
        {
            // Normal character
            if (*current != '\\')
            {
                workBuffer += *current++;
            }
            else
            {
                // Escape sequence
                current++;
                if (current != jsonString.end())
                {
                    // Single character
                    if (m_from.count(*current) > 0)
                    {
                        workBuffer += m_from[*current++];
                    }
                    // UTF16 "\uxxxx"
                    else if ((*current == 'u') && ((current + 4) < jsonString.end()))
                    {
                        char hexDigits[5] = {current[1], current[2], current[3], current[4], '\0'};
                        char *end;
                        workBuffer += static_cast<char16_t>(std::strtol(hexDigits, &end, 16));
                        if (*end != '\0')
                        {
                            throw JSONLib::SyntaxError();
                        }
                        current += 5; // Move paste the \uxxxx
                    }
                    else
                    {
                        throw JSONLib::SyntaxError();
                    }
                }
                else
                {
                    throw JSONLib::SyntaxError();
                }
            }
        }
        // Check that there are no single unpaired UTF-16 surrogates.From what I see this is
        // meant to be an error but from searching the web I have not found a definitive answer.
        int index = 0;
        while (index <= (static_cast<int>(workBuffer.size()) - 1))
        {
            if (isValidSurrogateUpper(workBuffer[index]) && isValidSurrogateLower(workBuffer[index + 1]))
            {
                index++;
            }
            else if (isValidSurrogateUpper(workBuffer[index]) || isValidSurrogateLower(workBuffer[index + 1]))
            {
                throw JSONLib::SyntaxError();
            }
            index++;
        }
        return (utf16_to_utf8(workBuffer));
    }
    /// <summary>
    /// Convert a string from raw charater values (UTF8) so that it has character
    /// escapes where applicable for its JSON form.
    /// </summary>
    /// <param name="utf8String">String to convert.</param>
    /// <returns>JSON string with escapes.</returns>
    std::string JSON_Translator::to(std::string const &utf8String)
    {
        std::string workBuffer;
        for (char16_t utf16char : utf8_to_utf16(utf8String))
        {
            // Control characters
            if (m_to.count(utf16char) > 0)
            {
                workBuffer += m_to[utf16char];
            }
            // ASCII
            else if ((utf16char > 0x1F) && (utf16char < 0x80))
            {
                workBuffer += static_cast<char>(utf16char);
            }
            // UTF8 escaped
            else
            {
                static const char *digits = "0123456789ABCDEF";
                workBuffer += "\\u";
                workBuffer += digits[(utf16char >> 12) & 0x0f];
                workBuffer += digits[(utf16char >> 8) & 0x0f];
                workBuffer += digits[(utf16char >> 4) & 0x0f];
                workBuffer += digits[(utf16char)&0x0f];
            }
        }
        return (workBuffer);
    }
} // namespace JSONLib
