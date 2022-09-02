//
// Unit Tests: JSONLib_Tests_JSON
//
// Description: JSON creation API(s) unit tests for JSON class
// using the Catch2 test framework. Note: These tests use more
// modern raw literal strings to represent JSON.
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
// Parse simple JSON types
// =======================
TEST_CASE("JSON object creation api.", "[JSON][Create][Object]")
{
  SECTION("Initialise JSON with JSON object passed to constructor.", "[JSON][Create][Constructor]")
  {
    REQUIRE_NOTHROW(JSON(R"({ "pi" : 3.141 })"));
  }
  SECTION(
    "Initialise JSON with JSON object passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
  {
    const JSON json(R"({ "pi" : 3.141 })");
    REQUIRE_FALSE(!json.root().isObject());
    REQUIRE_FALSE(!json.root()["pi"].isNumber());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).getNumber().getFloat(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a integer.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 300;
    REQUIRE_FALSE(!json["integer"].isNumber());
    REQUIRE(JRef<Number>(json.root()["integer"]).getNumber().getInt() == 300);
  }
  SECTION("Initialise root JSON JNode with one entry containing a long.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 30000l;
    REQUIRE_FALSE(!json["integer"].isNumber());
    REQUIRE(JRef<Number>(json.root()["integer"]).getNumber().getLong() == 30000);
  }
  SECTION("Initialise root JSON JNode with one entry containing a float.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141f;
    REQUIRE_FALSE(!json["pi"].isNumber());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).getNumber().getFloat(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a double.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141;
    REQUIRE_FALSE(!json["pi"].isNumber());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).getNumber().getDouble(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a const char *.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = "robert";
    REQUIRE_FALSE(!json["name"].isString());
    REQUIRE(JRef<String>(json.root()["name"]).getString() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a std::string.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = std::string{ "robert" };
    REQUIRE_FALSE(!json["name"].isString());
    REQUIRE(JRef<String>(json.root()["name"]).getString() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a boolean.", "[JSON][Create][Object][Boolean]")
  {
    JSON json;
    json["flag"] = true;
    REQUIRE_FALSE(!json["flag"].isBoolean());
    REQUIRE_FALSE(!JRef<Boolean>(json.root()["flag"]).getBoolean());
  }
  SECTION("Initialise root JSON JNode with one entry containing a null.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"] = nullptr;
    REQUIRE_FALSE(!json["nothing"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]).getNull() == nullptr);
  }
  SECTION("Create two level object and null at base.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"] = nullptr;
    REQUIRE_FALSE(!json["nothing"]["extra"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]).getNull() == nullptr);
  }
  SECTION("Create three level object and null at base and stringify.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!json["nothing"]["extra"]["more"].isNull());
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]["more"]).getNull() == nullptr);
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(json.stringify(destinationBuffer));
    REQUIRE(destinationBuffer.getBuffer() == R"({"nothing":{"extra":{"more":null}}})");
  }
}
TEST_CASE("JSON array creation api.", "[JSON][Create][Array]")
{
  SECTION("Initialise JSON with JSON array  passed to constructor.", "[JSON][Create][Constructor]")
  {
    REQUIRE_NOTHROW(JSON(R"([ "pi", 3.141 ])"));
  }
  SECTION(
    "Initialise JSON with JSON array passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
  {
    const JSON json(R"([ "pi", 3.141 ])");
    REQUIRE_FALSE(!json.root().isArray());
    REQUIRE_FALSE(!json.root()[0].isString());
    REQUIRE_FALSE(!json.root()[1].isNumber());
    REQUIRE(JRef<String>(json.root()[0]).getString() == "pi");
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[1]).getNumber().getFloat(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a integer.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 300;
    REQUIRE_FALSE(!json[0].isNumber());
    REQUIRE(JRef<Number>(json.root()[0]).getNumber().getInt() == 300);
  }
  SECTION("Initialise root JSON array with one entry containing a long.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 30000l;
    REQUIRE_FALSE(!json[0].isNumber());
    REQUIRE(JRef<Number>(json.root()[0]).getNumber().getLong() == 30000);
  }
  SECTION("Initialise root JSON array with one entry containing a float.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141f;
    REQUIRE_FALSE(!json[0].isNumber());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[0]).getNumber().getFloat(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a double.", "[JSON][Create][Array][Number]")
  {
    JSON json;
    json[0] = 3.141;
    REQUIRE_FALSE(!json[0].isNumber());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()[0]).getNumber().getDouble(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON array with one entry containing a const char *.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = "robert";
    REQUIRE_FALSE(!json[0].isString());
    REQUIRE(JRef<String>(json.root()[0]).getString() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a std::string.", "[JSON][Create][Array][String]")
  {
    JSON json;
    json[0] = std::string{ "robert" };
    REQUIRE_FALSE(!json[0].isString());
    REQUIRE(JRef<String>(json.root()[0]).getString() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a boolean.", "[JSON][Create][Array][Boolean]")
  {
    JSON json;
    json[0] = true;
    REQUIRE_FALSE(!json[0].isBoolean());
    REQUIRE_FALSE(!JRef<Boolean>(json.root()[0]).getBoolean());
  }
  SECTION("Initialise root JSON array with one entry containing a null.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0] = nullptr;
    REQUIRE_FALSE(!json[0].isNull());
    REQUIRE(JRef<Null>(json.root()[0]).getNull() == nullptr);
  }
  SECTION("Create two level array with null at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[0][0] = nullptr;
    REQUIRE_FALSE(!json[0][0].isNull());
    REQUIRE(JRef<Null>(json.root()[0][0]).getNull() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"([[null]])");
  }
  SECTION("Create array with free spaces string at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!json[5].isString());
    REQUIRE(JRef<String>(json.root()[5]).getString() == "test");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"([null,null,null,null,null,"test"])");
  }
  SECTION("Create array with free spaces add an number at the base and stringify.", "[JSON][Create][Array][null]")
  {
    JSON json;
    json[5] = "test";
    REQUIRE_FALSE(!json[5].isString());
    REQUIRE(JRef<String>(json.root()[5]).getString() == "test");
    json[3] = 15;
    REQUIRE_FALSE(!json[3].isNumber());
    REQUIRE(JRef<Number>(json.root()[3]).getNumber().getInt() == 15);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"([null,null,null,15,null,"test"])");
  }
  SECTION("Create array with initializer list assignment.", "[JSON][Create][Array][initializer]")
  {
    JSON json;
    json[5] = { 1.0, 2.0, 3, 4.333, "5.0", "test test test test", false, nullptr };
    REQUIRE_FALSE(!json[5][0].isNumber());
    REQUIRE_FALSE(!json[5][1].isNumber());
    REQUIRE_FALSE(!json[5][2].isNumber());
    REQUIRE_FALSE(!json[5][3].isNumber());
    REQUIRE_FALSE(!json[5][4].isString());
    REQUIRE_FALSE(!json[5][5].isString());
    REQUIRE_FALSE(!json[5][6].isBoolean());
    REQUIRE_FALSE(!json[5][7].isNull());
    REQUIRE(JRef<Array>(json[5]).size() == 8);
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][0]).getNumber().getDouble(), 1.0, 0.0001));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][1]).getNumber().getDouble(), 2.0, 0.0001));
    REQUIRE(JRef<Number>(json[5][2]).getNumber().getInt() == 3);
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json[5][3]).getNumber().getDouble(), 4.333, 0.0001));
    REQUIRE(JRef<String>(json[5][4]).getString() == "5.0");
    REQUIRE(JRef<String>(json[5][5]).getString() == "test test test test");
    REQUIRE_FALSE(JRef<Boolean>(json[5][6]).getBoolean());
    REQUIRE(JRef<Null>(json[5][7]).getNull() == nullptr);
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer()
            == R"([null,null,null,null,null,[1.0,2.0,3,4.333,"5.0","test test test test",false,null]])");
  }
}
TEST_CASE("JSON create complex JSON structures.", "[JSON][Create][Complex]")
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
    REQUIRE(jsonDestination.getBuffer() == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null})");
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
    REQUIRE(jsonDestination.getBuffer()
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
      jsonDestination.getBuffer()
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
      jsonDestination.getBuffer()
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
      jsonDestination.getBuffer()
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
      jsonDestination.getBuffer()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":{"key1":22,"key2":99.899}}})");
  }
  SECTION("Array creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { 1, 2, 3, 4 };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"([1,2,3,4])");
  }
  SECTION("Object creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"({"currency":"USD","value":42.99})");
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
      jsonDestination.getBuffer()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
}