#pragma once
#include <cwctype>
#include <string>
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
        using String = std::string;
        using Char = String::value_type;
        virtual ~ISource() = default;
        [[nodiscard]] virtual Char current() const = 0;
        virtual void next() = 0;
        [[nodiscard]] virtual bool more() const = 0;
        virtual void reset() = 0;
        // Ignore whitespace on source stream
        void ignoreWS()
        {
            while (more() && (std::iswspace(current()) != 0))
            {
                next();
            }
        }
    };
} // namespace JSONLib