#pragma once
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // Interface for utf8 <-> uft16 conversion 
    //
    class IConverter
    {
    public:
        virtual ~IConverter() = default;
        virtual std::u16string to_utf16(const std::string &utf8) = 0;
        virtual std::string to_utf8(const std::u16string &utf16) = 0;
    };
} // namespace JSONLib