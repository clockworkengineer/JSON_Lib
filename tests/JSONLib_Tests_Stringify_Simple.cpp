//
// Unit Tests: JSON
//
// Description: JSON stringification of simple type unit
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
// ==============================================
// Stringification of simple types and validation
// ==============================================
TEST_CASE("JSON object for stringification of simple types and check values",
          "[JSON][Stringify][Simple][Validate]") {
  const JSON json;
  SECTION("Stringify a string (Test string) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"\"Test string.\""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a string (Test another string) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"\"Test another string.\""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a boolean (true) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"true"};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a boolean (false) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"false"};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a number (98345) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"98345"};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a number (250000) and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"250000"};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a null and check its value",
          "[JSON][Stringify][Simple][Validate]") {
    const std::string expected{"null"};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}