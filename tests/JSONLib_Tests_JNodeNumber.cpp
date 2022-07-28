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
  const JSON json;
  // Note:: That floating point -> integer will be rounded up/down
  SECTION("Floating point converted to int", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE(JNodeRef<Number>(json.root()).number().getInt() == 6788889);
  }
  SECTION("Floating point converted to long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLong());
    REQUIRE(JNodeRef<Number>(json.root()).number().getLong() == 6788889);
  }
  SECTION("Floating point converted to long long",
          "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"6788888.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLLong());
    REQUIRE(JNodeRef<Number>(json.root()).number().getLong() == 6788889);
  }
  SECTION("Floating point converted to float", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getFloat(), 678.8990f,
        0.0001));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getDouble(), 678.8990,
        0.0001));
  }
  SECTION("Floating point converted to long double",
          "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLDouble());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getLDouble(), 678.899L,
        0.0001));
  }
  SECTION("Integer converted to int", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE(JNodeRef<Number>(json.root()).number().getInt() == 78989);
  }
  SECTION("Integer converted to long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLong());
    REQUIRE(JNodeRef<Number>(json.root()).number().getLong() == 78989);
  }
  SECTION("Integer converted to long long", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLLong());
    REQUIRE(JNodeRef<Number>(json.root()).number().getLLong() == 78989);
  }
  SECTION("Integer converted to float", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getFloat(), 78989.0f,
        0.0001));
  }
  SECTION("Integer converted to double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getDouble(), 78989.0,
        0.0001));
  }
  SECTION("Integer converted to long double", "[JSON][JNode][JNodeNumber]") {
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<Number>(json.root()).number().isInt());
    REQUIRE_FALSE(JNodeRef<Number>(json.root()).number().isLDouble());
    REQUIRE_FALSE(!equalFloatingPoint(
        JNodeRef<Number>(json.root()).number().getLDouble(), 78989.0L,
        0.0001));
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

TEST_CASE("Check JNodeNumeric api for all supported number types.",
          "[JSON][JNode][JNodeNumeric]") {
  JSON json;
  SECTION("Check numbers are the correct type.",
          "[JSON][JNode][JNodeNumeric][Addition]") {
    json["root"] = {1, 1l, 1ll, 1.0f, 1.0, 1.0L};
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][0]).number().isInt());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][1]).number().isLong());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][2]).number().isLLong());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][3]).number().isFloat());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][4]).number().isDouble());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][5]).number().isLDouble());
  }
  SECTION("Simple arithmetic add one to a number",
          "[JSON][JNode][JNodeNumeric][Get/Set]") {
    json["root"] = {1, 1l, 1ll, 1.0f, 1.0, 1.0L};
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][0])
             .number()
             .setInt(JNodeRef<Number>(json["root"][0]).number().getInt() +
                     1));
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][1])
             .number()
             .setLong(
                 JNodeRef<Number>(json["root"][1]).number().getLong() +
                 1));
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][2])
             .number()
             .setLLong(
                 JNodeRef<Number>(json["root"][2]).number().getLLong() +
                 1));
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][3])
             .number()
             .setFloat(
                 JNodeRef<Number>(json["root"][3]).number().getFloat() +
                 1.0f));
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][4])
             .number()
             .setDouble(
                 JNodeRef<Number>(json["root"][4]).number().getDouble() +
                 1.0));
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][5])
             .number()
             .setLDouble(
                 JNodeRef<Number>(json["root"][5]).number().getLDouble() +
                 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[2,2,2,2.0,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][JNodeNumeric][Reset]") {
    json["root"] = {1, 1l, 1ll, 1.0f, 1.0, 1.0L};
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][0]).number().isInt());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][1]).number().isLong());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][2]).number().isLLong());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][3]).number().isFloat());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][4]).number().isDouble());
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][5]).number().isLDouble());
    json["root"][2] = 3.0;
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][2]).number().isDouble());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() ==
            R"({"root":[1,1,3.0,1.0,1.0,1.0]})");
    REQUIRE_FALSE(
        !JNodeRef<Number>(json["root"][5]).number().setLLong(445ll));
    REQUIRE_FALSE(!JNodeRef<Number>(json["root"][5]).number().isLLong());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() ==
            R"({"root":[1,1,3.0,1.0,1.0,445]})");
  }
}