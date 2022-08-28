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
  SECTION("Print a simple array", "[JSON][Print][Array][Validate]")
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
  SECTION("Print a simple object", "[JSON][Print][Object][Validate]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male"
})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a nested array", "[JSON][Print][Array][Validate]")
  {
    const std::string expected{ R"([
    1,
    2,
    3,
    [
        5,
        6,
        7,
        8
    ],
    5,
    6,
    [
        4,
        5,
        6,
        7,
        8
    ]
])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a nested object", "[JSON][Print][Object][Validate]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male",
    "Details": {
        "Phone": "0999-999-999",
        "email": "john.doe@express.com",
        "enabled": true
    }
})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}