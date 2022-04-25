#pragma once
#include <cwctype>
namespace JSONLib
{
    //
    // Interface for reading source stream during Bencode deocoding.
    //
    class ISource
    {
    public:
        virtual ~ISource() {}
        virtual char current() const = 0;
        virtual void next() = 0;
        virtual bool more() const = 0;
        virtual void reset() = 0;
        // Is the current character whitespace
        bool isWS()  const
        {
            return (std::iswspace(current()));
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