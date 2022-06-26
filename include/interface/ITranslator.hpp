#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // =========================
    // JSON translator interface
    // =========================
    class ITranslator
    {
    public:
        // ========================
        // Constructors/destructors
        // ========================
        ITranslator() = default;
        ITranslator(const ITranslator &other) = delete;
        ITranslator &operator=(const ITranslator &other) = delete;
        ITranslator(ITranslator &&other) = delete;
        ITranslator &operator=(ITranslator &&other) = delete;
        virtual ~ITranslator() = default;
        // =====================================================================
        // Convert any escape sequences in a string to their correct sequence
        // of UTF-8 characters. If input string contains any unpaired surrogates
        // then this is deemed as a syntax error and an error is duly thrown.
        // =====================================================================
        virtual std::string from(const std::string &jsonString) = 0;
        // =========================================================================
        // Convert a string from raw charater values (UTF8) so that it has character
        // escapes where applicable for its JSON form.
        // =========================================================================
        virtual std::string to(const std::string &utf8String) = 0;
    protected:
        // =====================================================================================
        // Check that there are no single unpaired UTF-16 surrogates.From what I see this is
        // meant to be an error but from searching the web I have not found a definitive answer.
        //======================================================================================
        bool unpairedSurrogatesInBuffer(const std::u16string &utf16Buffer)
        {
            int index = 0;
            while (index <= (static_cast<int>(utf16Buffer.size()) - 1))
            {
                if (isValidSurrogateUpper(utf16Buffer[index]) && isValidSurrogateLower(utf16Buffer[index + 1]))
                {
                    index++;
                }
                else if (isValidSurrogateUpper(utf16Buffer[index]) || isValidSurrogateLower(utf16Buffer[index + 1]))
                {
                    return (true);
                }
                index++;
            }
            return (false);
        }
    private:
        bool isValidSurrogateUpper(char16_t c)
        {
            return ((c >= 0xD800) && (c <= 0xDBFF));
        }
        bool isValidSurrogateLower(char16_t c)
        {
            return ((c >= 0xDC00) && (c <= 0xDFFF));
        }
    };
} // namespace JSONLib