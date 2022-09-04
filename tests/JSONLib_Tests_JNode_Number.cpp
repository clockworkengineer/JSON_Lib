//
// Unit Tests: JSONLib_Tests_JNode_Number
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
TEST_CASE("Check JNode Number assignment/creation.", "[JSON][JNode][Number][Assignment]")
{
  SECTION("Create JNode with int assigment.", "[JSON][JNode][Number][Integer][Assignment]")
  {
    JNode jNode;
    jNode = 666;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isInt());
    REQUIRE(JRef<Number>(jNode).getNumber().getInt() == 666);
  }
  SECTION("Create JNode with long assigment.", "[JSON][JNode][Number][Long][Assignment]")
  {
    JNode jNode;
    jNode = 666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isLong());
    REQUIRE(JRef<Number>(jNode).getNumber().getLong() == 666l);
  }
  SECTION("Create JNode with long long assigment.", "[JSON][JNode][Number][Long Long][Assignment]")
  {
    JNode jNode;
    jNode = 666ll;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isLongLong());
    REQUIRE(JRef<Number>(jNode).getNumber().getLong() == 666ll);
  }
  SECTION("Create JNode with float assigment.", "[JSON][JNode][Number][Float][Assignment]")
  {
    JNode jNode;
    jNode = 666.666f;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).getNumber().getFloat(), 666.666f, 0.0001));
  }
  SECTION("Create JNode with double assigment.", "[JSON][JNode][Number][Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).getNumber().getDouble(), 666.666, 0.0001));
  }
  SECTION("Create JNode with long double assigment.", "[JSON][JNode][Number][Long Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isLongDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).getNumber().getLongDouble(), 666.666l, 0.0001));
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
  SECTION("Float converted to long long.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLongLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLongLong() == 6788889ll);
  }
  SECTION("Float converted to double.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getDouble(), 678.8990, 0.0001));
  }
  SECTION("Float converted to long double.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isFloat());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLongDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getLongDouble(), 678.8990l, 0.0001));
  }
  SECTION("Int converted to long.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLong() == 78989);
  }
  SECTION("Int converted to long long.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLongLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLongLong() == 78989ll);
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
  SECTION("Int converted to long double.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLongDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).getNumber().getLongDouble(), 78989.0l, 0.0001));
  }
  SECTION("Large positive integer conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "877994604561387500" };
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isLongLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLongLong() == 877994604561387500ll);
  }
  SECTION("Large negative integer conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "-877994604561387500" };
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isInt());
    REQUIRE_FALSE(JRef<Number>(json.root()).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json.root()).getNumber().isLongLong());
    REQUIRE(JRef<Number>(json.root()).getNumber().getLongLong() == -877994604561387500ll);
  }
  SECTION("Floating point precision.", "[JSON][JNode][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).getNumber().isFloat());
    BufferDestination jsonDestination;
    Numeric::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.0683,"longitude":-70.7416})");
    jsonDestination.clear();
    Numeric::setPrecision(7);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834,"longitude":-70.74162})");
    jsonDestination.clear();
    Numeric::setPrecision(8);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834,"longitude":-70.741615})");
    Numeric::setPrecision(6);
  }
  SECTION("Floating point notation.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).getNumber().isFloat());
    BufferDestination jsonDestination;
    Numeric::setNotation(Numeric::Notation::normal);
    Numeric::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.0683,"longitude":-70.7416})");
    Numeric::setNotation(Numeric::Notation::scientific);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":3.906834e+01,"longitude":-7.074162e+01})");
    Numeric::setNotation(Numeric::Notation::fixed);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.068340,"longitude":-70.741615})");
    Numeric::setNotation(Numeric::Notation::normal);
  }
}
// ==================================
// JNode Number conversion exceptions
// ==================================
TEST_CASE("Check JNode Number conversion exceptions.", "[JSON][JNode][Number][Exceptions]")
{
  const JSON json;
  SECTION("Check integer with invalid characters.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "4345u334u" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
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
// JNode Numeric numeric type API(s)
// ===============================
TEST_CASE("Check JNode Numeric API(s) for all supported number types.", "[JSON][JNode][Numeric]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][JNode][Numeric][Addition]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).getNumber().isInt());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).getNumber().isLongLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["root"][4]).getNumber().isDouble());
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).getNumber().isLongDouble());
  }
  SECTION("Simple arithmetic add one to a number.", "[JSON][JNode][Numeric][Get/Set]")
  {
    json["root"] = { 1, 1l, 1l, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    Numeric &integerRef = JRef<Number>(json["root"][0]).getNumber();
    REQUIRE_FALSE(!integerRef.setInt(integerRef.getInt() + 1));
    Numeric &longRef = JRef<Number>(json["root"][1]).getNumber();
    REQUIRE_FALSE(!longRef.setLong(longRef.getLong() + 1));
    Numeric &longLongRef = JRef<Number>(json["root"][2]).getNumber();
    REQUIRE_FALSE(!longLongRef.setLongLong(longLongRef.getLongLong() + 1));
    Numeric &floatRef = JRef<Number>(json["root"][3]).getNumber();
    REQUIRE_FALSE(!floatRef.setFloat(floatRef.getFloat() + 1.0f));
    Numeric &doubleRef = JRef<Number>(json["root"][4]).getNumber();
    REQUIRE_FALSE(!doubleRef.setDouble(doubleRef.getDouble() + 1.0));
    Numeric &longDoubleRef = JRef<Number>(json["root"][5]).getNumber();
    REQUIRE_FALSE(!longDoubleRef.setLongDouble(longDoubleRef.getLongDouble() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[2,2,2,2.0,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][Numeric][Reset]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).getNumber().isInt());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).getNumber().isLongLong());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).getNumber().isFloat());
    REQUIRE_FALSE(!JRef<Number>(json["root"][4]).getNumber().isDouble());
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).getNumber().isLongDouble());
    json["root"][1] = 3.0;
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).getNumber().isDouble());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).getNumber().setLong(445));
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).getNumber().isLong());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1,1.0,1.0,445]})");
  }
}