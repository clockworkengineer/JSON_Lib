//
// Unit Tests: JSON
//
// Description: JSON print of JSON unit tests for JSON class
// using the Catch2 test framework.
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
// ==============================================
// Stringification of simple types and validation
// ==============================================
TEST_CASE("Check printing of example JSON", "[JSON][Print]")
{
  const JSON json;
  SECTION("Print a simple array", "[JSON][Stringify][Simple][Validate]")
  {
    const std::string expected{ R"([
    1,
    2,
    3,
    4,
    5
])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}