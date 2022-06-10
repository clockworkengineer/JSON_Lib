#pragma once
#include <cwctype>
#include <string>
#include <codecvt>
#include <locale>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // Interface for reading source stream during JSON parsing.
    //
    class ISource
    {
    public:
        // 16 bit characters internally
        using String = std::u16string;
        using Char = String::value_type;
        // Virtual destructor
        virtual ~ISource() = default;
        // Current character
        [[nodiscard]] virtual Char current() const = 0;
        // Move to next character
        virtual void next() = 0;
        // Are there still more characters to read ?
        [[nodiscard]] virtual bool more() const = 0;
        // Reset to beginning of source stream
        virtual void reset() = 0;
        // Is the current character whitespace
        [[nodiscard]] bool isWS() const
        {
            return (std::iswspace(static_cast<wint_t>(current()))) != 0;
        }
        // Ignore whitespace on source stream
        void ignoreWS()
        {
            while (more() && isWS())
            {
                next();
            }
        }
    };
} // namespace JSONLib