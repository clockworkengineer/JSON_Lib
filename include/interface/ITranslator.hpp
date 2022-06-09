#pragma once
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // JSON translator interface
    //
    class ITranslator
    {
    public:
        virtual ~ITranslator() = default;
        virtual std::string from(const std::string &jsonString) = 0;
        virtual std::string to(const std::string &utf8String) = 0;

    protected:
        // Check that there are no single unpaired UTF-16 surrogates.From what I see this is
        // meant to be an error but from searching the web I have not found a definitive answer.
        bool unpairedSurrogatesInBuffer(const std::u16string &buffer)
        {
            int index = 0;
            while (index <= (static_cast<int>(buffer.size()) - 1))
            {
                if (isValidSurrogateUpper(buffer[index]) && isValidSurrogateLower(buffer[index + 1]))
                {
                    index++;
                }
                else if (isValidSurrogateUpper(buffer[index]) || isValidSurrogateLower(buffer[index + 1]))
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