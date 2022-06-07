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
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// JSON translator constructor.
    /// </summary>
    JSON_Translator::JSON_Translator(IConverter *converter)
    {
        // Initialise tables used to convert to/from single character
        // escape sequences within a JSON string.
        std::vector<std::pair<char, char16_t>> escapeSequences{
            {'\\', '\\'},
            {'t', '\t'},
            {'"', '\"'},
            {'/', '/'},
            {'b', '\b'},
            {'f', '\f'},
            {'n', '\n'},
            {'r', '\r'}};

        for (auto &[key, value] : escapeSequences)
        {
            m_from[key] = value;
            m_to[value] = key;
        }
        if (converter == nullptr)
        {
            m_converter = std::make_unique<JSON_Converter>();
        }
        else
        {
            m_converter.reset(converter);
        }
    }
    /// <summary>
    /// Convert any escape sequences in a string to their correct sequence
    //  of UTF-8 characters. If input string contains any unpaired surrogates
    //  then this is deemed as a syntax error and an error is duly thrown.
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
                    if (m_from.contains(*current))
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
        return (m_converter->utf16_to_utf8(workBuffer));
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
        for (char16_t utf16char : m_converter->utf8_to_utf16(utf8String))
        {
            // Control characters
            if (m_to.contains(utf16char))
            {
                workBuffer += '\\';
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
