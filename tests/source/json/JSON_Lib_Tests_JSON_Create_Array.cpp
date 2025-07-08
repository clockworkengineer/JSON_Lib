#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON array creation api.", "[JSON][Create][Array]")
{
  SECTION("Initialise JSON with JSON array  passed to constructor.", "[JSON][Create][Constructor]")
  {
    REQUIRE_NOTHROW(JSON(R"([ "pi", 3.141 ])"));
  }
  SECTION(
    "Initialise JSON with JSON array passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
  {
    const JSON json(R"([ "pi", 3.141 ])");
    REQUIRE_FALSE(!isA<Array>(json.root()));
    REQUIRE_FALSE(!isA<String>(json.root()[0]));
    REQUIRE_FALSE(!isA<Number>(json.root()[1]));
    REQUIRE(NRef<String>(json.root()[0]).value() == "pi");
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()[1]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a integer.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 300;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(NRef<Number>(json.root()[0]).value<int>() == 300);
  }
  SECTION("Initialise root JSON array with one entry containing a long.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 30000l;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(NRef<Number>(json.root()[0]).value<long>() == 30000);
  }
  SECTION("Initialise root JSON array with one entry containing a long long.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 30000ll;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(NRef<Number>(json.root()[0]).value<long long>() == 30000);
  }
  SECTION("Initialise root JSON array with one entry containing a float.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141f;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()[0]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a double.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141l;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()[0]).value<long double>(), 3.141l, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a long double.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()[0]).value<double>(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a const char *.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = "robert";
    REQUIRE_FALSE(!isA<String>(json[0]));
    REQUIRE(NRef<String>(json.root()[0]).value() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a std::string.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = std::string{ "robert" };
    REQUIRE_FALSE(!isA<String>(json[0]));
    REQUIRE(NRef<String>(json.root()[0]).value() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a boolean.", "[JSON][Create][Array][Boolean]")
  {
    JSON json;
    json[0] = true;
    REQUIRE_FALSE(!isA<Boolean>(json[0]));
    REQUIRE_FALSE(!NRef<Boolean>(json.root()[0]).value());
  }
  SECTION("Initialise root JSON array with one entry containing a null.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json[0]));
    REQUIRE(NRef<Null>(json.root()[0]).value() == nullptr);
  }
  SECTION("Create two level array with null at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0][0] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json[0][0]));
    REQUIRE(NRef<Null>(json.root()[0][0]).value() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([[null]])");
  }
  SECTION("Create array with free spaces string at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!isA<String>(json[5]));
    REQUIRE(NRef<String>(json.root()[5]).value() == "test");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([null,null,null,null,null,"test"])");
  }
  SECTION("Create array with free spaces add an number at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!isA<String>(json[5]));
    REQUIRE(NRef<String>(json.root()[5]).value() == "test");
    json[3] = 15;
    REQUIRE_FALSE(!isA<Number>(json[3]));
    REQUIRE(NRef<Number>(json.root()[3]).value<int>() == 15);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([null,null,null,15,null,"test"])");
  }
  SECTION("Create array with initializer list assignment.", "[JSON][Create][Array][initializer]")
  {
    JSON json;
    json[5] = { 1.0, 2.0, 3, 4.333, "5.0", "test test test test", false, nullptr };
    REQUIRE_FALSE(!isA<Number>(json[5][0]));
    REQUIRE_FALSE(!isA<Number>(json[5][1]));
    REQUIRE_FALSE(!isA<Number>(json[5][2]));
    REQUIRE_FALSE(!isA<Number>(json[5][3]));
    REQUIRE_FALSE(!isA<String>(json[5][4]));
    REQUIRE_FALSE(!isA<String>(json[5][5]));
    REQUIRE_FALSE(!isA<Boolean>(json[5][6]));
    REQUIRE_FALSE(!isA<Null>(json[5][7]));
    REQUIRE(NRef<Array>(json[5]).size() == 8);
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json[5][0]).value<double>(), 1.0, 0.0001));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json[5][1]).value<double>(), 2.0, 0.0001));
    REQUIRE(NRef<Number>(json[5][2]).value<int>() == 3);
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json[5][3]).value<double>(), 4.333, 0.0001));
    REQUIRE(NRef<String>(json[5][4]).value() == "5.0");
    REQUIRE(NRef<String>(json[5][5]).value() == "test test test test");
    REQUIRE_FALSE(NRef<Boolean>(json[5][6]).value());
    REQUIRE(NRef<Null>(json[5][7]).value() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString()
            == R"([null,null,null,null,null,[1.0,2.0,3,4.333,"5.0","test test test test",false,null]])");
  }
}