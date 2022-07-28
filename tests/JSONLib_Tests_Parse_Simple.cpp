//
// Unit Tests: JSON
//
// Description: JSON parse simple types unit tests for JSON class
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
// =======================
// Parse simple JSON types
// =======================
TEST_CASE("JSON object for parse of simple types (number,string,boolean,null).",
          "[JSON][Parse][Simple]") {
  const JSON json;
  SECTION("Parse an string (example string)", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"\"example string\""};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::string);
  }
  SECTION("Parse an number (6767)", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"6767"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::number);
  }
  SECTION("Parse an boolean (true)", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"true"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::boolean);
  }
  SECTION("Parse an boolean (false)", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"false"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::boolean);
  }
  SECTION("Parse an null", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"null"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::null);
  }
  SECTION("Parse an empty string", "[JSON][Parse][Simple]") {
    BufferSource jsonSource{"\"\""};
    json.parse(jsonSource);
    REQUIRE((json.root()).getNodeType() == JNodeType::string);
  }
}
// ====================================
// Parse simple JSON types and validate
// ====================================
TEST_CASE("JSON object for parse of simple types and check values.",
          "[JSON][Parse][Simple][Validate]") {
  const JSON json;
  SECTION("Parse an string (example string) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"\"example string\""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() == "example string");
  }
  SECTION("Parse an string (another example string) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"\"another example string\""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<String>(json.root()).string() ==
            "another example string");
  }
  SECTION("Parse an number (6767) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"6767"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<Number>(json.root()).toString() == "6767");
  }
  SECTION("Parse an number (190000) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"190000"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<Number>(json.root()).toString() == "190000");
  }
  SECTION("Parse an boolean (true) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"true"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<Boolean>(json.root()).boolean() == true);
  }
  SECTION("Parse an boolean (false) and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"false"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<Boolean>(json.root()).boolean() == false);
  }
  SECTION("Parse an null and check its value",
          "[JSON][Parse][Simple][Validate]") {
    BufferSource jsonSource{"null"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<Null>(json.root()).null() == nullptr);
  }
}