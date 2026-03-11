#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check Node Number conversion exceptions.", "[JSON][Node][Number][Exceptions]")
{
  const JSON json;
  SECTION("Check integer with invalid characters.", "[JSON][Node][Number][Exceptions]")
  {
    BufferSource jsonSource{ "4345u334u" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with exponent.", "[JSON][Node][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.43e-2" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent.", "[JSON][Node][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.e43e-2" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 10]: Invalid numeric value.");
  }
  SECTION("Check floating point with multiple decimal points.", "[JSON][Node][Number][Exceptions]")
  {
    BufferSource jsonSource{ "78.5454.545" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 12]: Invalid numeric value.");
  }
  SECTION("Scientific notation with negative exponent parses to correct value.", "[JSON][Node][Number]")
  {
    // 78.43e-2 = 0.7843
    json.parse(BufferSource{ "78.43e-2" });
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), 0.7843f, 0.0001));
  }
  SECTION("Scientific notation with positive exponent parses to correct value.", "[JSON][Node][Number]")
  {
    // 1e2 = 100; stored as float because 'e' prevents integer parsing
    json.parse(BufferSource{ "1e2" });
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), 100.0f, 0.001));
  }
  SECTION("Scientific notation negative value parses to correct value.", "[JSON][Node][Number]")
  {
    // -5.5e1 = -55
    json.parse(BufferSource{ "-5.5e1" });
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), -55.0f, 0.001));
  }
}

TEST_CASE("Check Node Number API(s) for all supported number types.", "[JSON][Node][Number]")
{
  JSON json;
  SECTION("Check numbers are the correct type.", "[JSON][Node][Number][Addition]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!NRef<Number>(json["root"][0]).is<int>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][1]).is<long>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][2]).is<long long>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][3]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][4]).is<double>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][5]).is<long double>());
  }
  SECTION("Simple arithmetic add one to a number.", "[JSON][Node][Number][Get/Set]")
  {
    json["root"] = { 1, 1l, 1l, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    Number &integerRef = NRef<Number>(json["root"][0]);
    REQUIRE_NOTHROW(integerRef.set(integerRef.value<int>() + 1));
    Number &longRef = NRef<Number>(json["root"][1]);
    REQUIRE_NOTHROW(longRef.set(longRef.value<long>() + 1));
    Number &longLongRef = NRef<Number>(json["root"][2]);
    REQUIRE_NOTHROW(longLongRef.set(longLongRef.value<long long>() + 1));
    Number &floatRef = NRef<Number>(json["root"][3]);
    REQUIRE_NOTHROW(floatRef.set(floatRef.value<float>() + 1.0f));
    Number &doubleRef = NRef<Number>(json["root"][4]);
    REQUIRE_NOTHROW(doubleRef.set(doubleRef.value<double>() + 1.0));
    Number &longDoubleRef = NRef<Number>(json["root"][5]);
    REQUIRE_NOTHROW(longDoubleRef.set(longDoubleRef.value<long double>() + 1.0));
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[2,2,2,2.0,2.0,2.0]})");
  }
  SECTION("Change types and values.", "[JSON][Node][Number][Reset]")
  {
    json["root"] = { 1, 1l, 1ll, 1.0f, 1.0, 1.0l };
    BufferDestination destinationBuffer;
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,1,1,1.0,1.0,1.0]})");
    REQUIRE_FALSE(!NRef<Number>(json["root"][0]).is<int>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][1]).is<long>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][2]).is<long long>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][3]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][4]).is<double>());
    REQUIRE_FALSE(!NRef<Number>(json["root"][5]).is<long double>());
    json["root"][1] = 3.0;
    REQUIRE_FALSE(!NRef<Number>(json["root"][1]).is<double>());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,3.0,1,1.0,1.0,1.0]})");
    REQUIRE_NOTHROW(NRef<Number>(json["root"][5]).set(445l));
    REQUIRE_FALSE(!NRef<Number>(json["root"][5]).is<long>());
    destinationBuffer.clear();
    json.stringify(destinationBuffer);
    REQUIRE(destinationBuffer.toString() == R"({"root":[1,3.0,1,1.0,1.0,445]})");
  }
}

TEST_CASE("Check Number setPrecision() and setNotation().", "[JSON][Node][Number]")
{
  SECTION("Default precision (6) produces expected float string.", "[JSON][Node][Number]")
  {
    Number::setPrecision(6);
    Number::setNotation(Number::numberNotation::normal);
    Node jNode(1.5f);
    REQUIRE(NRef<Number>(jNode).toString() == "1.5");
  }
  SECTION("setPrecision(4) with fixed notation produces trailing zeros.", "[JSON][Node][Number]")
  {
    Number::setPrecision(4);
    Number::setNotation(Number::numberNotation::fixed);
    Node jNode(1.5f);
    const std::string result = NRef<Number>(jNode).toString();
    Number::setPrecision(6);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(result == "1.5000");
  }
  SECTION("setNotation(scientific) produces output containing 'e'.", "[JSON][Node][Number]")
  {
    Number::setPrecision(4);
    Number::setNotation(Number::numberNotation::scientific);
    Node jNode(1.5f);
    const std::string result = NRef<Number>(jNode).toString();
    Number::setPrecision(6);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(result.find('e') != std::string::npos);
  }
  SECTION("Integer node toString() is not affected by float notation.", "[JSON][Node][Number]")
  {
    Number::setPrecision(4);
    Number::setNotation(Number::numberNotation::fixed);
    Node jNode(42);
    const std::string result = NRef<Number>(jNode).toString();
    Number::setPrecision(6);
    Number::setNotation(Number::numberNotation::normal);
    // Integers always format without decimal notation settings
    REQUIRE(result == "42");
  }
  SECTION("setPrecision(2) with normal notation.", "[JSON][Node][Number]")
  {
    Number::setPrecision(2);
    Number::setNotation(Number::numberNotation::normal);
    Node jNode(3.14f);
    const std::string result = NRef<Number>(jNode).toString();
    Number::setPrecision(6);
    // precision 2 on 3.14 in defaultfloat should give "3.1"
    REQUIRE(result == "3.1");
  }
}