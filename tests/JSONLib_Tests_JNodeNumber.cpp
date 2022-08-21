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
TEST_CASE("Check JNodeNumber number conversion", "[JSON][JNode][JNodeNumber]")
{
  const JSON json;
  // Note:: That floating point -> integer will be rounded up/down
  SECTION("Floating point converted to int", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE(JRef<Number>(json.root()).getNumber().getInt() == 6788889);
  }
  SECTION("Floating point converted to long", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLong() == 6788889);
  }
  SECTION("Floating point converted to float", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getFloat(), 678.8990f, 0.0001));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getDouble(), 678.8990, 0.0001));
  }
  SECTION("Integer converted to int", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE(JRef<Number>(json.root()).getNumber().getInt() == 78989);
  }
  SECTION("Integer converted to long", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLong() == 78989);
  }
  SECTION("Integer converted to float", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getFloat(), 78989.0f, 0.0001));
  }
  SECTION("Integer converted to double", "[JSON][JNode][JNodeNumber]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getDouble(), 78989.0, 0.0001));
  }
  SECTION("Check  floating point with exponent", "[JSON][JNode][JNodeNumber][Exception")
  {
    BufferSource jsonSource{ "78.43e-2" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{ "78.e43e-2" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Invalid numeric value.");
  }
  SECTION("Check floating point with multiple decimal points", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{ "78.5454.545" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Invalid numeric value.");
  }
}
TEST_CASE("Check JNodeNumeric api for all supported number types.", "[JSON][JNode][JNodeNumeric]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][JNode][JNodeNumeric][Addition]")
  {
    json["root"] = { 1, 1l, 1.0f, 1.0 };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).getNumber().isInt());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().isDouble());
  }
  SECTION("Simple arithmetic add one to a number", "[JSON][JNode][JNodeNumeric][Get/Set]")
  {
    json["root"] = { 1, 1l, 1.0f, 1.0 };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1.0,1.0]})");
    REQUIRE_FALSE(
      !JRef<Number>(json["root"][0]).getNumber().setInt(JRef<Number>(json["root"][0]).getNumber().getInt() + 1));
    REQUIRE_FALSE(
      !JRef<Number>(json["root"][1]).getNumber().setLong(JRef<Number>(json["root"][1]).getNumber().getLong() + 1));
    REQUIRE_FALSE(
      !JRef<Number>(json["root"][2]).getNumber().setFloat(JRef<Number>(json["root"][2]).getNumber().getFloat() + 1.0f));
    REQUIRE_FALSE(!JRef<Number>(json["root"][3])
                     .getNumber()
                     .setDouble(JRef<Number>(json["root"][3]).getNumber().getDouble() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[2,2,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][JNodeNumeric][Reset]")
  {
    json["root"] = { 1, 1l, 1.0f, 1.0 };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).getNumber().isInt());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().isDouble());
    json["root"][1] = 3.0;
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isDouble());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().setLong(445));
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().isLong());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1.0,445]})");
  }
}