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
        [[nodiscard]] static bool isValidSurrogateUpper(char16_t c)
        {
            return ((c >= 0xD800) && (c <= 0xDBFF));
        }
        [[nodiscard]] static bool isValidSurrogateLower(char16_t c)
        {
            return ((c >= 0xDC00) && (c <= 0xDFFF));
        }
    };
} // namespace JSONLib