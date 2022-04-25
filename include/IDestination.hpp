#pragma once
//
// C++ STL
//
#include <string>
namespace JSONLib
{
    //
    // Interface for writing destination stream during Bencode encoding.
    //
    class IDestination
    {
    public:
        virtual ~IDestination() {}
        virtual void add(const std::string &bytes) = 0;
    };
} // namespace JSONLib
