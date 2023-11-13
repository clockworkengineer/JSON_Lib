//
// Unit Tests: JSON_Lib_Tests_JNode_Number
//
// Description: JNode Number/Number class unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// =======================
// JNode Number assignment
// =======================
TEST_CASE("Check JNode Number assignment/creation.", "[JSON][JNode][Number][Assignment]")
{
  SECTION("Create JNode with int assigment.", "[JSON][JNode][Number][Integer][Assignment]")
  {
    JNode jNode;
    jNode = 666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).get<int>() == 666);
  }
  SECTION("Create JNode with long assigment.", "[JSON][JNode][Number][Long][Assignment]")
  {
    JNode jNode;
    jNode = 666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).get<long>() == 666l);
  }
  SECTION("Create JNode with long long assigment.", "[JSON][JNode][Number][Long Long][Assignment]")
  {
    JNode jNode;
    jNode = 666ll;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).get<long>() == 666ll);
  }
  SECTION("Create JNode with float assigment.", "[JSON][JNode][Number][Float][Assignment]")
  {
    JNode jNode;
    jNode = 666.666f;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).get<float>(), 666.666f, 0.0001));
  }
  SECTION("Create JNode with double assigment.", "[JSON][JNode][Number][Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).get<double>(), 666.666, 0.0001));
  }
  SECTION("Create JNode with long double assigment.", "[JSON][JNode][Number][Long Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).get<long double>(), 666.666l, 0.0001));
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
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<int>());
    REQUIRE(JRef<Number>(json.root()).get<int>() == 6788889);
  }
  SECTION("Float converted to long.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
    REQUIRE(JRef<Number>(json.root()).get<long>() == 6788889);
  }
  SECTION("Float converted to long long.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    REQUIRE(JRef<Number>(json.root()).get<long long>() == 6788889ll);
  }
  SECTION("Float converted to double.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<double>(), 678.8990, 0.0001));
  }
  SECTION("Float converted to long double.", "[JSON][JNode][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<long double>(), 678.8990l, 0.0001));
  }
  SECTION("Int converted to long.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
    REQUIRE(JRef<Number>(json.root()).get<long>() == 78989);
  }
  SECTION("Int converted to long long.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    REQUIRE(JRef<Number>(json.root()).get<long long>() == 78989ll);
  }
  SECTION("Int converted to float.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<float>(), 78989.0f, 0.0001));
  }
  SECTION("Int converted to double.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<double>(), 78989.0, 0.0001));
  }
  SECTION("Int converted to long double.", "[JSON][JNode][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<long double>(), 78989.0l, 0.0001));
  }
  SECTION("Large positive integer conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "9223372036854775807" };
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).is<int>());
    // If long and long long same size then integer never stored in long long
    // but you can fetch it as a long long
    if constexpr (sizeof(long long) > sizeof(long)) {
      REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(!JRef<Number>(json.root()).is<long long>());
    } else {
      REQUIRE_FALSE(!JRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    }
    REQUIRE(JRef<Number>(json.root()).get<long long>() == 9223372036854775807ll);
  }
  SECTION("Large negative integer conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "-877994604561387500" };
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).is<int>());
    // If long and long long same size then integer never stored in long long
    // but you can fetch it as a long long
    if constexpr (sizeof(long long) > sizeof(long)) {
      REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(!JRef<Number>(json.root()).is<long long>());
    } else {
      REQUIRE_FALSE(!JRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    }
    REQUIRE(JRef<Number>(json.root()).get<long long>() == -877994604561387500ll);
  }
  SECTION("Positive integer overflow conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "9223372036854775808" };// LLONG_MAX + 1
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<float>(), 9223372036854775808.0f, 0.0001));
  }
  SECTION("Negative integer overflow conversion.", "[JSON][JNode][Number][Long Long]")
  {
    BufferSource jsonSource{ "-9223372036854775809" };// LLONG_MIN - 1
    json.parse(jsonSource);
    REQUIRE_FALSE(JRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long>());
    REQUIRE_FALSE(JRef<Number>(json.root()).is<long long>());
    REQUIRE_FALSE(!JRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()).get<float>(), -9223372036854775808.0f, 0.0001));
  }
}
// =====================================
// JNode Number floating point precision
// =====================================
TEST_CASE("Check JNode Number floating point precision.", "[JSON][JNode][Number][Float][Precision]")
{
  const JSON json;
  SECTION("Floating point precision to 6 (default).", "[JSON][JNode][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.0683,"longitude":-70.7416})");
  }
  SECTION("Floating point precision to 7.", "[JSON][JNode][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(7);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834,"longitude":-70.74162})");
  }
  SECTION("Floating point precision to 8.", "[JSON][JNode][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(8);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834,"longitude":-70.741615})");
  }
  SECTION("Floating point precision to maximum.", "[JSON][JNode][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(std::numeric_limits<long double>::digits10 + 1);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    if constexpr ((std::numeric_limits<long double>::digits10 + 1) == 16) {
      REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834030151367,"longitude":-70.74161529541016})");
    } else if constexpr ((std::numeric_limits<long double>::digits10 + 1) == 19) {
      REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.06834030151367188,"longitude":-70.74161529541015625})");
    } else {
      REQUIRE_FALSE(true);
    }
  }
}
// ====================================
// JNode Number floating point notation
// ====================================
TEST_CASE("Check JNode Number floating point notation.", "[JSON][JNode][Number][Float][Precision]")
{
  const JSON json;
  SECTION("Floating point notation to normal.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.0683,"longitude":-70.7416})");
  }
  SECTION("Floating point notation to fixed.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::fixed);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":39.068340,"longitude":-70.741615})");
  }
  SECTION("Floating point notation to scientific.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.getBuffer() == R"({"latitude":3.906834e+01,"longitude":-7.074162e+01})");
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
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSON_Lib::JSON::Error);
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
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSON_Lib::JSON::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with multiple decimal points.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.5454.545" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSON_Lib::JSON::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error [Line: 1 Column: 12]: Invalid numeric value.");
  }
}
// ===============================
// JNode Number numeric type API(s)
// ===============================
TEST_CASE("Check JNode Number API(s) for all supported number types.", "[JSON][JNode][Number]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][JNode][Number][Addition]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).is<int>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).is<long>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).is<long long>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][4]).is<double>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).is<long double>());
  }
  SECTION("Simple arithmetic add one to a number.", "[JSON][JNode][Number][Get/Set]")
  {
    json["root"] = { 1, 1l, 1l, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    Number &integerRef = JRef<Number>(json["root"][0]);
    REQUIRE_NOTHROW(integerRef.set(integerRef.get<int>() + 1));
    Number &longRef = JRef<Number>(json["root"][1]);
    REQUIRE_NOTHROW(longRef.set(longRef.get<long>() + 1));
    Number &longLongRef = JRef<Number>(json["root"][2]);
    REQUIRE_NOTHROW(longLongRef.set(longLongRef.get<long long>() + 1));
    Number &floatRef = JRef<Number>(json["root"][3]);
    REQUIRE_NOTHROW(floatRef.set(floatRef.get<float>() + 1.0f));
    Number &doubleRef = JRef<Number>(json["root"][4]);
    REQUIRE_NOTHROW(doubleRef.set(doubleRef.get<double>() + 1.0));
    Number &longDoubleRef = JRef<Number>(json["root"][5]);
    REQUIRE_NOTHROW(longDoubleRef.set(longDoubleRef.get<long double>() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[2,2,2,2.0,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][Number][Reset]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!JRef<Number>(json["root"][0]).is<int>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).is<long>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][2]).is<long long>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][3]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][4]).is<double>());
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).is<long double>());
    json["root"][1] = 3.0;
    REQUIRE_FALSE(!JRef<Number>(json["root"][1]).is<double>());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1,1.0,1.0,1.0]})");
    REQUIRE_NOTHROW(JRef<Number>(json["root"][5]).set(445l));
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).is<long>());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.getBuffer() == R"({"root":[1,3.0,1,1.0,1.0,445]})");
  }
}