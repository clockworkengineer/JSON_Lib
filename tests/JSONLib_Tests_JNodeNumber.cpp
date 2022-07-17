//
// Unit Tests: JSON
//
// Description: JNodeNumber unit tests for JSON class
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

// ===========
// JNodeNumber
// ===========
TEST_CASE("Check JNodeNumber number conversion", "[JSON][JNode][JNodeNumber]") {
  JSON json;
  SECTION("Floating point converted to int", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(json.root()).number().isInt());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getInt() == 6788888);
  }
  SECTION("Floating point converted to long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(json.root()).number().isLong());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLong() == 6788888);
  }
  SECTION("Floating point converted to long long",
          "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(json.root()).number().isLongLong());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLong() == 6788888);
  }
  SECTION("Floating point converted to float", "[JSON][JNode][JNodeNumber]") {
    // auto compare = [](float a, float b, float epsilon =
    // std::numeric_limits<float>::epsilon()){ return (std::fabs(a - b) <=
    // epsilon); };
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isFloat());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getFloat() ==
            static_cast<float>(678.8990));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isDouble());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getDouble() ==
            678.8990);
  }
  SECTION("Floating point converted to long double",
          "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isLongDouble());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLongDouble() ==
            678.8990);
  }
  SECTION("Integer converted to int", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isInt());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getInt() == 78989);
  }
  SECTION("Integer converted to long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isLong());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLong() == 78989);
  }
  SECTION("Integer converted to long long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isLongLong());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLongLong() == 78989);
  }
  SECTION("Integer converted to float", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isFloat());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getFloat() == 78989.0);
  }
  SECTION("Integer converted to double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isDouble());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getDouble() == 78989.0);
  }
  SECTION("Integer converted to long double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).number().isLongDouble());
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number().getLongDouble() ==
            78989.0);
  }
  SECTION("Check  floating point with exponent",
          "[JSON][JNode][JNodeNumber][Exception") {
    BufferSource jsonSource{"78.43e-2"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent",
          "[JSON][JNode][JNodeNumber][Exception]") {
    BufferSource jsonSource{"78.e43e-2"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Check floating point with multiple decimal points",
          "[JSON][JNode][JNodeNumber][Exception]") {
    BufferSource jsonSource{"78.5454.545"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
}
