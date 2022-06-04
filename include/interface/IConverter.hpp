#pragma once
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // Character utf8 <-> uft16 conversion interface
    //
    class IConverter
    {
    public:
        virtual ~IConverter() = default;
        virtual std::u16string utf8_to_utf16(const std::string &utf8) = 0;
        virtual std::string utf16_to_utf8(const std::u16string &utf16) = 0;
    };
} // namespace JSONLib