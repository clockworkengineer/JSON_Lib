//
// Unit Tests: JSON
//
// Description: JSON stringification collection  types unit 
// tests for JSON class using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSONLib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
// ==================================================
// Stringification of collection types and validation
// ==================================================
TEST_CASE("JSON object for stringification of collection types and check values.", "[JSON][Stringify]")
{
    JSON json;
    SECTION("Stringify an object ({\"Age\":77,\"Name\":\"Rob\"}) and check its value", "[JSON][Stringify]")
    {
        const std::string expected{"{\"Age\":77,\"Name\":\"Rob\"}"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([999,\"Time\",null,true] and check its value", "[JSON][Stringify]")
    {
        const std::string expected{"[999,\"Time\",null,true]"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an object ({\"City\":\"London\",\"Population\":8000000}) and check its value", "[JSON][Stringify]")
    {
        const std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([true,\"Out of time\",789043e13,true]) and check its value", "[JSON][Stringify]")
    {
        const std::string expected{"[true,\"Out of time\",789043e13,true]"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}
