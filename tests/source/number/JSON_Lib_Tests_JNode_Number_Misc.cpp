#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JNode Number conversion exceptions.", "[JSON][JNode][Number][Exceptions]")
{
  const JSON json;
  SECTION("Check integer with invalid characters.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "4345u334u" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with exponent.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.43e-2" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.e43e-2" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with multiple decimal points.", "[JSON][JNode][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.5454.545" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 12]: Invalid numeric value.");
  }
}

TEST_CASE("Check JNode Number API(s) for all supported number types.", "[JSON][JNode][Number]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][JNode][Number][Addition]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
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
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    Number &integerRef = JRef<Number>(json["root"][0]);
    REQUIRE_NOTHROW(integerRef.set(integerRef.value<int>() + 1));
    Number &longRef = JRef<Number>(json["root"][1]);
    REQUIRE_NOTHROW(longRef.set(longRef.value<long>() + 1));
    Number &longLongRef = JRef<Number>(json["root"][2]);
    REQUIRE_NOTHROW(longLongRef.set(longLongRef.value<long long>() + 1));
    Number &floatRef = JRef<Number>(json["root"][3]);
    REQUIRE_NOTHROW(floatRef.set(floatRef.value<float>() + 1.0f));
    Number &doubleRef = JRef<Number>(json["root"][4]);
    REQUIRE_NOTHROW(doubleRef.set(doubleRef.value<double>() + 1.0));
    Number &longDoubleRef = JRef<Number>(json["root"][5]);
    REQUIRE_NOTHROW(longDoubleRef.set(longDoubleRef.value<long double>() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[2,2,2,2.0,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][JNode][Number][Reset]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
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
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,3.0,1,1.0,1.0,1.0]})");
    REQUIRE_NOTHROW(JRef<Number>(json["root"][5]).set(445l));
    REQUIRE_FALSE(!JRef<Number>(json["root"][5]).is<long>());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,3.0,1,1.0,1.0,445]})");
  }
}