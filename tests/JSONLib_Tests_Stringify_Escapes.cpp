//
// Unit Tests: JSON
//
// Description: JSON stringification of escaped character
// unit tests for JSON class using the Catch2 test framework.
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

// =====================================
// Stringification of escaped characters
// =====================================
TEST_CASE("JSON object for stringification of strings with escape characters.",
          "[JSON][Stringify][Escapes]") {
  const JSON json;
  SECTION("Stringify JSON string with escapes '/' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String / \\t \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    REQUIRE(JRef<String>(json.root()).string() == "Test String / \t ");
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\t' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\t \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\\"' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\\" \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\\\' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\\\ \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\b' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\b \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\/' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    // Escaped normal ascii is not re-scaped for stringification
    const std::string expected{"\"Test String \\/ \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "\"Test String / \"");
  }
  SECTION("Stringify JSON string with escapes '\\f' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\f \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\n' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\n \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\r' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\r \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\t' to buffer and check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\t \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(
      "Stringify JSON string with escapes '\\u0123' to buffer and check value",
      "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\u0123 \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify JSON string with escapes '\\u0123 \\u0456' to buffer and "
          "check value",
          "[JSON][Stringify][Escapes]") {
    const std::string expected{"\"Test String \\u0123 \\u0456 \""};
    BufferDestination jsonDestination;
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}