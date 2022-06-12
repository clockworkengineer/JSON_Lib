#pragma once
#include <string>
#include "ISource.hpp"
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
        virtual std::string from(const ISource::String &jsonString) = 0;
        virtual std::string to(const std::string &utf8String) = 0;
    protected:
        // Check that there are no single unpaired UTF-16 surrogates.From what I see this is
        // meant to be an error but from searching the web I have not found a definitive answer.
        bool unpairedSurrogatesInBuffer(const ISource::String &utf16Buffer)
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
        bool isValidSurrogateUpper(ISource::Char c)
        {
            return ((c >= 0xD800) && (c <= 0xDBFF));
        }
        bool isValidSurrogateLower(ISource::Char  c)
        {
            return ((c >= 0xDC00) && (c <= 0xDFFF));
        }
    };
} // namespace JSONLib