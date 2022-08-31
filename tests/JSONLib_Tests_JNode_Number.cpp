//
// Unit Tests: JSONLib
//
// Description: JNode Number/Numeric class unit tests for JSON class
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
// JNode Number assignment
// =======================
TEST_CASE("Check JNode Number assignment.", "[JSON][JNode][Number][Assignment]")
{
  JSON json;
  SECTION("Create array element with int in first element.", "[JSON][JNode][Number][Integer]")
  {
    json[0] = 666;
    REQUIRE_FALSE(!JRef<Number>(json[0]).getNumber().isInt());
    REQUIRE(JRef<Number>(json[0]).getNumber().getInt() == 666);
  }
  SECTION("Create array element with long in second element.", "[JSON][JNode][Number][Long]")
  {
    json[1] = 666l;
    REQUIRE_FALSE(!JRef<Number>(json[1]).getNumber().isLong());
    REQUIRE(JRef<Number>(json[1]).getNumber().getLong() == 666);
  }
  SECTION("Create array element with float in third element.", "[JSON][JNode][Number][Float]")
  {
    json[2] = 666.666f;
    REQUIRE_FALSE(!JRef<Number>(json[2]).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[2]).getNumber().getFloat(), 666.666f, 0.0001));
  }
  SECTION("Create array element with double in fourth element.", "[JSON][JNode][Number][Double]")
  {
    json[3] = 666.666;
    REQUIRE_FALSE(!JRef<Number>(json[3]).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[3]).getNumber().getDouble(), 666.666, 0.0001));
  }
}
// =======================
// JNode Number conversion
// =======================
TEST_CASE("Check JNode Number conversion.", "[JSON][JNode][Number]")
{
  const JSON json;
  // Note: That floating point -> integer will be rounded up/down
  SECTION("Float converted to int.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE(JRef<Number>(json.root()).getNumber().getInt() == 6788889);
  }
  SECTION("Float converted to long.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLong() == 6788889);
  }
  SECTION("Float converted to float.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getFloat(), 678.8990f, 0.0001));
  }
  SECTION("Float converted to double.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getDouble(), 678.8990, 0.0001));
  }
  SECTION("Int converted to int.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE(JRef<Number>(json.root()).getNumber().getInt() == 78989);
  }
  SECTION("Int converted to long.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLong() == 78989);
  }
  SECTION("Int converted to float.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getFloat(), 78989.0f, 0.0001));
  }
  SECTION("Int converted to double.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getDouble(), 78989.0, 0.0001));
  }
}
// ==================================
// JNode Number conversion exceptions
// ==================================
TEST_CASE("Check JNode Number conversion exceptions.", "[JSON][JNode][Number][Exceptions]")
{
  const JSON json;
  SECTION("Check floating point with exponent.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.43e-2" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.e43e-2" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with multiple decimal points.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.5454.545" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error [Line: 1 Column: 12]: Invalid numeric value.");
  }
}
// ===============================
// JNode Numeric numeric type apis
// ===============================
TEST_CASE("Check JNode Numeric api for all supported number types.", "[JSON][JNode][Numeric]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][JNode][Numeric][Addition]")
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
  SECTION("Simple arithmetic add one to a number.", "[JSON][JNode][Numeric][Get/Set]")
  {
    json["root"] = { 1, 1l, 1.0f, 1.0 };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1.0,1.0]})");
    Numeric &integerRef = JRef<Number>(json["root"][0]).getNumber();
    REQUIRE_FALSE(!integerRef.setInt(integerRef.getInt() + 1));
    Numeric &longRef = JRef<Number>(json["root"][1]).getNumber();
    REQUIRE_FALSE(!longRef.setLong(longRef.getLong() + 1));
    Numeric &floatRef = JRef<Number>(json["root"][2]).getNumber();
    REQUIRE_FALSE(!floatRef.setFloat(floatRef.getFloat() + 1.0f));
    Numeric &doubleRef = JRef<Number>(json["root"][3]).getNumber();
    REQUIRE_FALSE(!doubleRef.setDouble(doubleRef.getDouble() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[2,2,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][Numeric][Reset]")
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