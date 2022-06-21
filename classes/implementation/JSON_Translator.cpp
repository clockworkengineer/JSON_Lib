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
    static const std::vector<std::pair<const char, const ISource::Char>> escapeSequences{
        {'\\', '\\'},
        {'t', '\t'},
        {'"', '\"'},
        {'/', '/'},
        {'b', '\b'},
        {'f', '\f'},
        {'n', '\n'},
        {'r', '\r'}};
    // =======================
    // PUBLIC STATIC VARIABLES
    // =======================
    // ===============
    // PRIVATE METHODS
    // ===============
    /// <summary>
    /// Convert \uxxxx escape sequences in a string to their correct sequence
    //  of UTF-8 characters.
    /// </summary>
    /// <param name="current">Current character position.</param>
    /// <param name="numberOfCharacters">Number of characters left in source string.</param>
    /// <returns>UTF16 character for "\uxxx".</returns>
    ISource::Char decodeUTF16(ISource::String::const_iterator &current, ptrdiff_t numberOfCharacters)
    {
        if (numberOfCharacters >= 4)
        {
            ISource::Char utf16value{};
            // Hex digits will be ascii so can throw away high order byte of ISource::Char
            std::array<char, 5> hexDigits{static_cast<char>(current[1]),
                                          static_cast<char>(current[2]),
                                          static_cast<char>(current[3]),
                                          static_cast<char>(current[4]), '\0'};
            char *end;
            utf16value += static_cast<ISource::Char>(std::strtol(hexDigits.data(), &end, 16));
            if (*end == '\0')
            {
                current += hexDigits.size();
                return (utf16value);
            }
        }
        throw JSONLib::SyntaxError("Syntax error detected.");
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// JSON translator constructor.
    /// </summary>
    JSON_Translator::JSON_Translator(IConverter &converter) : m_converter(converter)
    {
        // Initialise tables used to convert to/from single character
        // escape sequences within a JSON string.
        for (const auto &[key, value] : escapeSequences)
        {
            m_from[key] = value;
            m_to[value] = key;
        }
    }
    /// <summary>
    /// Convert any escape sequences in a string to their correct sequence
    //  of UTF-8 characters. If input string contains any unpaired surrogates
    //  then this is deemed as a syntax error and an error is duly thrown.
    /// </summary>
    /// <param name="jsonString">JSON string to process.</param>
    /// <returns>String with escapes translated.</returns>
    std::string JSON_Translator::from(const ISource::String &jsonString)
    {
        ISource::String utf16Buffer;
        for (auto current = jsonString.begin(); current != jsonString.end();)
        {
            // Normal character
            if (*current != '\\')
            {
                utf16Buffer += *current++;
                continue;
            }
            current++;
            // Check escape sequence if characters to process
            if (current != jsonString.end())
            {
                // Single character
                if (m_from.contains(static_cast<char>(*current)))
                {
                    utf16Buffer += m_from[static_cast<char>(*current)];
                    current++;
                    continue;
                }
                // UTF16 "\uxxxx"
                if (*current == 'u')
                {
                    utf16Buffer += decodeUTF16(current, std::distance(current, jsonString.end()));
                    continue;
                }
            }
            throw JSONLib::SyntaxError("Syntax error detected.");
        }
        if (unpairedSurrogatesInBuffer(utf16Buffer))
        {
            throw JSONLib::SyntaxError("Syntax error detected.");
        }
        return (m_converter.to_utf8(utf16Buffer));
    }
    /// <summary>
    /// Convert a string from raw charater values (UTF8) so that it has character
    /// escapes where applicable for its JSON form.
    /// </summary>
    /// <param name="utf8String">String to convert.</param>
    /// <returns>JSON string with escapes.</returns>
    std::string JSON_Translator::to(const std::string &utf8String)
    {
        std::string utf8Buffer;
        for (ISource::Char utf16Char : m_converter.to_utf16(utf8String))
        {
            // Control characters
            if (m_to.contains(utf16Char))
            {
                utf8Buffer += '\\';
                utf8Buffer += m_to[utf16Char];
            }
            // ASCII
            else if ((utf16Char > 0x1F) && (utf16Char < 0x80))
            {
                utf8Buffer += static_cast<char>(utf16Char);
            }
            // UTF8 escaped
            else
            {
                static const char *digits = "0123456789ABCDEF";
                utf8Buffer += "\\u";
                utf8Buffer += digits[(utf16Char >> 12) & 0x0f];
                utf8Buffer += digits[(utf16Char >> 8) & 0x0f];
                utf8Buffer += digits[(utf16Char >> 4) & 0x0f];
                utf8Buffer += digits[(utf16Char)&0x0f];
            }
        }
        return (utf8Buffer);
    }
} // namespace JSONLib
