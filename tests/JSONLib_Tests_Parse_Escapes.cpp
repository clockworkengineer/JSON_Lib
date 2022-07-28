//
// Unit Tests: JSON
//
// Description: JSON parse escape character unit tests for JSON class
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
// =================================
// Parse JSON With Escape Characters
// =================================
TEST_CASE("JSON object for decoding of strings with escape characters.",
          "[JSON][Parse][Escapes]") {
  const JSON json;
  SECTION("Stringify JSON string with escapes '\\t' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\t \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \t ");
  }
  SECTION("Stringify JSON string with escapes '\\\"' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\\" \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \" ");
  }
  SECTION("Stringify JSON string with escapes '\\\\' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\\\ \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \\ ");
  }
  SECTION("Stringify JSON string with escapes '\\b' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\b \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \b ");
  }
  SECTION("Stringify JSON string with escapes '\\/' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\/ \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String / ");
  }
  SECTION("Stringify JSON string with escapes '\\f' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\f \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \f ");
  }
  SECTION("Stringify JSON string with escapes '\\n' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\n \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \n ");
  }
  SECTION("Stringify JSON string with escapes '\\r' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\r \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \r ");
  }
  SECTION("Stringify JSON string with escapes '\\t' to buffer and check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\t \""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "Test String \t ");
  }
  SECTION(
      "Stringify JSON string with escapes '\\u0123' to buffer and check value",
      "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\u0123 \""};
    json.parse(jsonSource);
    std::u8string expected{u8"Test String \u0123 "};
    REQUIRE(JNodeRef<String>(json.root()).string() ==
            std::string{expected.begin(), expected.end()});
  }
  SECTION("Stringify JSON string with escapes '\\u0123 \\u0456' to buffer and "
          "check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String \\u0123 \\u0456 \""};
    json.parse(jsonSource);
    std::u8string expected{u8"Test String \u0123 \u0456 "};
    REQUIRE(JNodeRef<String>(json.root()).string() ==
            std::string{expected.begin(), expected.end()});
  }
  SECTION("Stringify JSON string with escapes  '\\uD834\\uDD1E' to buffer and "
          "check value",
          "[JSON][Parse][Escapes]") {
    BufferSource jsonSource{"\"Test String  \\uD834\\uDD1E \""};
    json.parse(jsonSource);
    std::u8string expected{u8"Test String  \U0001D11E "};
    REQUIRE(JNodeRef<String>(json.root()).string() ==
            std::string{expected.begin(), expected.end()});
  }
}