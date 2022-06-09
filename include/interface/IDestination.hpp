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
        virtual void add(const std::string &bytes) = 0;
        virtual void add(char ch) = 0;
    };
} // namespace JSONLib
