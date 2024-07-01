//
// Unit Tests: JSON_Lib_Tests_JSON
//
// Description: JSON creation API(s) unit tests for JSON class
// using the Catch2 test framework. Note: These tests use more
// modern raw literal strings to represent JSON.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// =======================
// Parse simple JSON types
// =======================
TEST_CASE("Check JSON object creation api.", "[JSON][Create][Object]")
{
  SECTION("Initialise JSON with JSON object passed to constructor.", "[JSON][Create][Constructor]")
  {
    REQUIRE_NOTHROW(JSON(R"({ "pi" : 3.141 })"));
  }
  SECTION(
    "Initialise JSON with JSON object passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
  {
    const JSON json(R"({ "pi" : 3.141 })");
    REQUIRE_FALSE(!isA<Object>(json.root()));
    REQUIRE_FALSE(!isA<Number>(json.root()["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a integer.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 300;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(JRef<Number>(json.root()["integer"]).value<int>() == 300);
  }
  SECTION("Initialise root JSON JNode with one entry containing a long.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 30000l;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(JRef<Number>(json.root()["integer"]).value<long>() == 30000);
  }
  SECTION("Initialise root JSON JNode with one entry containing a float.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141f;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a double.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<double>(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a const char *.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = "robert";
    REQUIRE_FALSE(!json["name"].isString());
    REQUIRE(JRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a std::string.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = std::string{ "robert" };
    REQUIRE_FALSE(!json["name"].isString());
    REQUIRE(JRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a boolean.", "[JSON][Create][Object][Boolean]")
  {
    JSON json;
    json["flag"] = true;
    REQUIRE_FALSE(!json["flag"].isBoolean());
    REQUIRE_FALSE(!JRef<Boolean>(json.root()["flag"]).value());
  }
  SECTION("Initialise root JSON JNode with one entry containing a null.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"] = nullptr;
    REQUIRE_FALSE(!json["nothing"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]).value() == nullptr);
  }
  SECTION("Create two level object and null at base.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"] = nullptr;
    REQUIRE_FALSE(!json["nothing"]["extra"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]).value() == nullptr);
  }
  SECTION("Create three level object and null at base and stringify.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!json["nothing"]["extra"]["more"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]["more"]).value() == nullptr);
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(json.stringify(destinationBuffer));
    REQUIRE(destinationBuffer.toString() == R"({"nothing":{"extra":{"more":null}}})");
  }
}
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
    REQUIRE_FALSE(!json.root()[0].isString());
    REQUIRE_FALSE(!isA<Number>(json.root()[1]));
    REQUIRE(JRef<String>(json.root()[0]).value() == "pi");
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[1]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a integer.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 300;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(JRef<Number>(json.root()[0]).value<int>() == 300);
  }
  SECTION("Initialise root JSON array with one entry containing a long.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 30000l;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(JRef<Number>(json.root()[0]).value<long>() == 30000);
  }
  SECTION("Initialise root JSON array with one entry containing a long long.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 30000ll;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE(JRef<Number>(json.root()[0]).value<long long>() == 30000);
  }
  SECTION("Initialise root JSON array with one entry containing a float.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141f;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[0]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a double.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141l;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[0]).value<long double>(), 3.141l, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a long double.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141;
    REQUIRE_FALSE(!isA<Number>(json[0]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[0]).value<double>(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a const char *.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = "robert";
    REQUIRE_FALSE(!json[0].isString());
    REQUIRE(JRef<String>(json.root()[0]).value() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a std::string.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = std::string{ "robert" };
    REQUIRE_FALSE(!json[0].isString());
    REQUIRE(JRef<String>(json.root()[0]).value() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a boolean.", "[JSON][Create][Array][Boolean]")
  {
    JSON json;
    json[0] = true;
    REQUIRE_FALSE(!json[0].isBoolean());
    REQUIRE_FALSE(!JRef<Boolean>(json.root()[0]).value());
  }
  SECTION("Initialise root JSON array with one entry containing a null.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0] = nullptr;
    REQUIRE_FALSE(!json[0].isNull());
    REQUIRE(JRef<Null>(json.root()[0]).value() == nullptr);
  }
  SECTION("Create two level array with null at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0][0] = nullptr;
    REQUIRE_FALSE(!json[0][0].isNull());
    REQUIRE(JRef<Null>(json.root()[0][0]).value() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([[null]])");
  }
  SECTION("Create array with free spaces string at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!json[5].isString());
    REQUIRE(JRef<String>(json.root()[5]).value() == "test");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([null,null,null,null,null,"test"])");
  }
  SECTION("Create array with free spaces add an number at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!json[5].isString());
    REQUIRE(JRef<String>(json.root()[5]).value() == "test");
    json[3] = 15;
    REQUIRE_FALSE(!isA<Number>(json[3]));
    REQUIRE(JRef<Number>(json.root()[3]).value<int>() == 15);
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
    REQUIRE_FALSE(!json[5][4].isString());
    REQUIRE_FALSE(!json[5][5].isString());
    REQUIRE_FALSE(!json[5][6].isBoolean());
    REQUIRE_FALSE(!json[5][7].isNull());
    REQUIRE(JRef<Array>(json[5]).size() == 8);
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][0]).value<double>(), 1.0, 0.0001));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][1]).value<double>(), 2.0, 0.0001));
    REQUIRE(JRef<Number>(json[5][2]).value<int>() == 3);
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][3]).value<double>(), 4.333, 0.0001));
    REQUIRE(JRef<String>(json[5][4]).value() == "5.0");
    REQUIRE(JRef<String>(json[5][5]).value() == "test test test test");
    REQUIRE_FALSE(JRef<Boolean>(json[5][6]).value());
    REQUIRE(JRef<Null>(json[5][7]).value() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString()
            == R"([null,null,null,null,null,[1.0,2.0,3,4.333,"5.0","test test test test",false,null]])");
  }
}
TEST_CASE("Check JSON create complex JSON structures.", "[JSON][Create][Complex]")
{
  SECTION("A Single level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null})");
  }
  SECTION("A two level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString()
            == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42}})");
  }
  SECTION("A three level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"][5] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"][5] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":[null,null,null,null,null,"Niels"],"nothing":null,"answer":{"everything":[null,null,null,null,null,42]}})");
  }
  SECTION("Object with sub array/object create using initializer list.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
  SECTION(
    "Object with sub array/object with an embedded array create using "
    "initializer list.",
    "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", JNode{ 1, 2, 3, 4, 5 } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":[1,2,3,4,5]}})");
  }
  SECTION(
    "Object with sub array/object with an embedded object create using "
    "initializer list.",
    "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", JNode{ { "key1", 22 }, { "key2", 99.899 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":{"key1":22,"key2":99.899}}})");
  }
  SECTION("Array creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { 1, 2, 3, 4 };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([1,2,3,4])");
  }
  SECTION("Object creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"({"currency":"USD","value":42.99})");
  }
  SECTION("Object creation completely using a nested initializer list.", "[JSON][Create][Complex][Initializer")
  {
    // Note: For the moment has to explicitly uses JNode to create a
    // nested object/array
    JSON json = { { "pi", 3.141 },
      { "happy", true },
      { "name", "Niels" },
      { "nothing", nullptr },
      { "answer", JNode{ { "everything", 42 } } },
      { "list", JNode{ 1, 0, 2 } },
      { "object", JNode{ { "currency", "USD" }, { "value", 42.99 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
}