//
// Unit Tests: JSON
//
// Description: JSON creation api unit tests for JSON class
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
TEST_CASE("JSON object creation api.", "[JSON][Create][Object]") {
  SECTION("Initialise JSON with JSON object  passed to constructor.",
          "[JSON][Create][Constructor]") {
    REQUIRE_NOTHROW(JSON{R"({ "pi" : 3.141 })"});
  }
  SECTION(
      "Initialise JSON with JSON object  passed to constructor and validate.",
      "[JSON][Create][Constructor][Validate]") {
    const JSON json{R"({ "pi" : 3.141 })"};
    REQUIRE(json.root().getType() == JNodeType::object);
    REQUIRE(json.root()["pi"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["pi"]).toString() == "3.141");
  }
  SECTION("Initialise root JSON object with one entry containing a integer.",
          "[JSON][Create][Object][Number]") {
    JSON json;
    json["integer"] = 300;
    REQUIRE(json["integer"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["integer"]).toString() == "300");
  }
  SECTION("Initialise root JSON object with one entry containing a long.",
          "[JSON][Create][Object][Number]") {
    JSON json;
    json["integer"] = 30000l;
    REQUIRE(json["integer"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["integer"]).toString() ==
            "30000");
  }
  SECTION("Initialise root JSON object with one entry containing a long long .",
          "[JSON][Create][Object][Number]") {
    JSON json;
    json["integer"] = 30000000ll;
    REQUIRE(json["integer"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["integer"]).toString() ==
            "30000000");
  }
  SECTION("Initialise root JSON object with one entry containing a float.",
          "[JSON][Create][Object][Number]") {
    JSON json;
    json["pi"] = 3.141f;
    REQUIRE(json["pi"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["pi"]).toString() == "3.141");
  }
  SECTION("Initialise root JSON object with one entry containing a double.",
          "[JSON][Create][Object][Number]") {
    JSON json;
    json["pi"] = 3.141;
    REQUIRE(json["pi"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["pi"]).toString() == "3.141");
  }
  SECTION(
      "Initialise root JSON object with one entry containing a long double.",
      "[JSON][Create][Object][Number]") {
    JSON json;
    json["pi"] = 3.141L;
    REQUIRE(json["pi"].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()["pi"]).toString() == "3.141");
  }
  SECTION(
      "Initialise root JSON object with one entry containing a const char *.",
      "[JSON][Create][Object][String]") {
    JSON json;
    json["name"] = "robert";
    REQUIRE(json["name"].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()["name"]).toString() == "robert");
  }
  SECTION(
      "Initialise root JSON object with one entry containing a std::string.",
      "[JSON][Create][Object][String]") {
    JSON json;
    json["name"] = std::string{"robert"};
    REQUIRE(json["name"].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()["name"]).toString() == "robert");
  }
  SECTION("Initialise root JSON object with one entry containing a boolean.",
          "[JSON][Create][Object][Boolean]") {
    JSON json;
    json["flag"] = true;
    REQUIRE(json["flag"].getType() == JNodeType::boolean);
    REQUIRE(JRef<Boolean>(json.root()["flag"]).toString() == "true");
  }
  SECTION("Initialise root JSON object with one entry containing a null.",
          "[JSON][Create][Object][null]") {
    JSON json;
    json["nothing"] = nullptr;
    REQUIRE(json["nothing"].getType() == JNodeType::null);
    REQUIRE(JRef<Null>(json.root()["nothing"]).toString() == "null");
  }
  SECTION("Create two level object and null at base.",
          "[JSON][Create][Object][null]") {
    JSON json;
    json["nothing"]["extra"] = nullptr;
    REQUIRE(json["nothing"]["extra"].getType() == JNodeType::null);
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]).toString() ==
            "null");
  }
  SECTION("Create three level object and null at base and stringify.",
          "[JSON][Create][Object][null]") {
    JSON json;
    json["nothing"]["extra"]["more"] = nullptr;
    REQUIRE(json["nothing"]["extra"]["more"].getType() == JNodeType::null);
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]["more"])
                .toString() == "null");
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(json.stringify(destinationBuffer));
    REQUIRE(destinationBuffer.getBuffer() ==
            R"({"nothing":{"extra":{"more":null}}})");
  }
}
TEST_CASE("JSON array creation api.", "[JSON][Create][Array]") {
  SECTION("Initialise JSON with JSON array  passed to constructor.",
          "[JSON][Create][Constructor]") {
    REQUIRE_NOTHROW(JSON{R"([ "pi", 3.141 ])"});
  }
  SECTION("Initialise JSON with JSON array passed to constructor and validate.",
          "[JSON][Create][Constructor][Validate]") {
    const JSON json{R"([ "pi", 3.141 ])"};
    REQUIRE(json.root().getType() == JNodeType::array);
    REQUIRE(json.root()[0].getType() == JNodeType::string);
    REQUIRE(json.root()[1].getType() == JNodeType::number);
    REQUIRE(JRef<String>(json.root()[0]).toString() == "pi");
    REQUIRE(JRef<Number>(json.root()[1]).toString() == "3.141");
  }
  SECTION("Initialise root JSON array with one entry containing a integer.",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 300;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "300");
  }
  SECTION("Initialise root JSON array with one entry containing a long.",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 30000l;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "30000");
  }
  SECTION("Initialise root JSON array with one entry containing a long long .",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 30000000ll;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "30000000");
  }
  SECTION("Initialise root JSON array with one entry containing a float.",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 3.141f;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "3.141");
  }
  SECTION("Initialise root JSON array with one entry containing a double.",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 3.141;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "3.141");
  }
  SECTION("Initialise root JSON array with one entry containing a long double.",
          "[JSON][Create][Array][Number]") {
    JSON json;
    json[0] = 3.141L;
    REQUIRE(json[0].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[0]).toString() == "3.141");
  }
  SECTION(
      "Initialise root JSON array with one entry containing a const char *.",
      "[JSON][Create][Array][String]") {
    JSON json;
    json[0] = "robert";
    REQUIRE(json[0].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()[0]).toString() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a std::string.",
          "[JSON][Create][Array][String]") {
    JSON json;
    json[0] = std::string{"robert"};
    REQUIRE(json[0].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()[0]).toString() == "robert");
  }
  SECTION("Initialise root JSON array with one entry containing a boolean.",
          "[JSON][Create][Array][Boolean]") {
    JSON json;
    json[0] = true;
    REQUIRE(json[0].getType() == JNodeType::boolean);
    REQUIRE(JRef<Boolean>(json.root()[0]).toString() == "true");
  }
  SECTION("Initialise root JSON array with one entry containing a null.",
          "[JSON][Create][Array][null]") {
    JSON json;
    json[0] = nullptr;
    REQUIRE(json[0].getType() == JNodeType::null);
    REQUIRE(JRef<Null>(json.root()[0]).toString() == "null");
  }
  SECTION("Create two level array with null at the base and stringify.",
          "[JSON][Create][Array][null]") {
    JSON json;
    json[0][0] = nullptr;
    REQUIRE(json[0][0].getType() == JNodeType::null);
    REQUIRE(JRef<Null>(json.root()[0][0]).toString() == "null");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() == R"([[null]])");
  }
  SECTION("Create array with free spaces string at the base and stringify.",
          "[JSON][Create][Array][null]") {
    JSON json;
    json[5] = "test";
    REQUIRE(json[5].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()[5]).toString() == "test");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() ==
            R"([null,null,null,null,null,"test"])");
  }
  SECTION(
      "Create array with free spaces add an number at the base and stringify.",
      "[JSON][Create][Array][null]") {
    JSON json;
    json[5] = "test";
    REQUIRE(json[5].getType() == JNodeType::string);
    REQUIRE(JRef<String>(json.root()[5]).toString() == "test");
    json[3] = 15;
    REQUIRE(json[3].getType() == JNodeType::number);
    REQUIRE(JRef<Number>(json.root()[3]).toString() == "15");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() ==
            R"([null,null,null,15,null,"test"])");
  }
  SECTION("Create array with initializer list assignment.",
          "[JSON][Create][Array][initializer]") {
    JSON json;
    json[5] = {1.0,   2.0,    3, 4.333, "5.0", "test test test test",
               false, nullptr};
    REQUIRE(json[5][0].getType() == JNodeType::number);
    REQUIRE(json[5][1].getType() == JNodeType::number);
    REQUIRE(json[5][2].getType() == JNodeType::number);
    REQUIRE(json[5][3].getType() == JNodeType::number);
    REQUIRE(json[5][4].getType() == JNodeType::string);
    REQUIRE(json[5][5].getType() == JNodeType::string);
    REQUIRE(json[5][6].getType() == JNodeType::boolean);
    REQUIRE(json[5][7].getType() == JNodeType::null);
    REQUIRE(JRef<Array>(json[5]).size() == 8);
    REQUIRE(JRef<Number>(json[5][0]).toString() == "1.0");
    REQUIRE(JRef<Number>(json[5][1]).toString() == "2.0");
    REQUIRE(JRef<Number>(json[5][2]).toString() == "3");
    REQUIRE(JRef<Number>(json[5][3]).toString() == "4.333");
    REQUIRE(JRef<String>(json[5][4]).toString() == "5.0");
    REQUIRE(JRef<String>(json[5][5]).toString() ==
            "test test test test");
    REQUIRE(JRef<Boolean>(json[5][6]).toString() == "false");
    REQUIRE(JRef<Null>(json[5][7]).toString() == "null");
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
        jsonDestination.getBuffer() ==
        R"([null,null,null,null,null,[1.0,2.0,3,4.333,"5.0","test test test test",false,null]])");
  }
}
TEST_CASE("JSON create complex JSON structures", "[JSON][Create][Complex]") {
  SECTION("A Single level object.", "[JSON][Create][Complex]") {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.getBuffer() ==
            R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null})");
  }
  SECTION("A two level object.", "[JSON][Create][Complex]") {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
        jsonDestination.getBuffer() ==
        R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42}})");
  }
  SECTION("A three level object.", "[JSON][Create][Complex]") {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"][5] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"][5] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
        jsonDestination.getBuffer() ==
        R"({"pi":3.141,"happy":true,"name":[null,null,null,null,null,"Niels"],"nothing":null,"answer":{"everything":[null,null,null,null,null,42]}})");
  }
  SECTION("Object with sub array/object create using initializer list.",
          "[JSON][Create][Complex]") {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = {1, 0, 2};
    json["object"] = {{"currency", "USD"}, {"value", 42.99}};
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
        jsonDestination.getBuffer() ==
        R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
  // Currently not supported but on TODO.
  // SECTION("Object creation completely using a nested initializer list.",
  //         "[JSON][Create][Complex][Initializer") {
  //   JSON json = {{"pi", 3.141},
  //                {"happy", true},
  //                {"name", "Niels"},
  //                {"nothing", nullptr},
  //                {"answer", {{"everything", 42}}},
  //                {"list", {1, 0, 2}},
  //                {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  //   BufferDestination jsonDestination;
  //   REQUIRE_NOTHROW(json.stringify(jsonDestination));
  //   REQUIRE(
  //       jsonDestination.getBuffer() ==
  //       R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  // }
}