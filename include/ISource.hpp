#pragma once
#include <cwctype>
namespace JSONLib
{
    //
    // Interface for reading source stream during Bencode decodings.
    //
    class ISource
    {
    public:
        virtual ~ISource() = default;
        [[nodiscard]] virtual char current() const = 0;
        virtual void next() = 0;
        [[nodiscard]] virtual bool more() const = 0;
        virtual void reset() = 0;
        // Is the current character whitespace
        [[nodiscard]] bool isWS()  const
        {
            return (std::iswspace(current())) != 0;
        }
        // Ignore shitespace on source stream
        void ignoreWS()
        {
            while (more() && isWS())
            {
                next();
            }
        }
    };
} // namespace JSONLib