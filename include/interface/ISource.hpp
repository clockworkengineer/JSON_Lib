#pragma once
#include <cwctype>
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
        virtual ~ISource() = default;
        [[nodiscard]] virtual char current() const = 0;
        virtual void next() = 0;
        [[nodiscard]] virtual bool more() const = 0;
        virtual void reset() = 0;
        // Ignore shitespace on source stream
        void ignoreWS()
        {
            while (more() && (std::iswspace(current()) != 0))
            {
                next();
            }
        }
    };
} // namespace JSONLib