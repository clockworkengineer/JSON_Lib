#pragma once
//
// C++ STL
//
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // Interface for writing destination stream during JSON stringification.
    //
    class IDestination
    {
    public:
        // Virtual destructor
        virtual ~IDestination() = default;
        // Add bytes to destination
        virtual void add(const ISource::String &bytes) = 0;
        virtual void add(ISource::Char) = 0;
    };
} // namespace JSONLib
