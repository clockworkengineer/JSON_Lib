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
TEST_CASE("JSON object creation api.", "[JSON][Create][Object]")
{
    SECTION("Initialise JSON with JSON passed to constructor.", "[JSON][Create][Constructor]")
    {
        REQUIRE_NOTHROW(JSON{R"({ "pi" : 3.141 })"});
    }
    SECTION("Initialise JSON with JSON string passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
    {
        const JSON json{R"({ "pi" : 3.141 })"};
        REQUIRE(json.root().getNodeType() == JNodeType::object);
        REQUIRE(json.root()["pi"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["pi"]).toString() == "3.141");
    }
    SECTION("Initialise root JSON object with one entry containing a integer.", "[JSON][Create][Number]")
    {
        JSON json;
        json["integer"] = 300;
        REQUIRE(json["integer"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["integer"]).toString() == "300");
    }
    SECTION("Initialise root JSON object with one entry containing a long.", "[JSON][Create][Number]")
    {
        JSON json;
        json["integer"] = 30000l;
        REQUIRE(json["integer"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["integer"]).toString() == "30000");
    }
    SECTION("Initialise root JSON object with one entry containing a long long .", "[JSON][Create][Number]")
    {
        JSON json;
        json["integer"] = 30000000ll;
        REQUIRE(json["integer"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["integer"]).toString() == "30000000");
    }
    SECTION("Initialise root JSON object with one entry containing a float.", "[JSON][Create][Number]")
    {
        JSON json;
        json["pi"] = 3.141f;
        REQUIRE(json["pi"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["pi"]).toString() == "3.141");
    }
    SECTION("Initialise root JSON object with one entry containing a double.", "[JSON][Create][Number]")
    {
        JSON json;
        json["pi"] = 3.141;
        REQUIRE(json["pi"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["pi"]).toString() == "3.141");
    }
    SECTION("Initialise root JSON object with one entry containing a long double.", "[JSON][Create][Number]")
    {
        JSON json;
        json["pi"] = 3.141L;
        REQUIRE(json["pi"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["pi"]).toString() == "3.141");
    }
    SECTION("Initialise root JSON object with one entry containing a const char *.", "[JSON][Create][String]")
    {
        JSON json;
        json["name"] = "robert";
        REQUIRE(json["name"].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()["name"]).toString() == "robert");
    }
    SECTION("Initialise root JSON object with one entry containing a std::string.", "[JSON][Create][String]")
    {
        JSON json;
        json["name"] = std::string{"robert"};
        REQUIRE(json["name"].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()["name"]).toString() == "robert");
    }
    SECTION("Initialise root JSON object with one entry containing a boolean.", "[JSON][Create][Boolean]")
    {
        JSON json;
        json["flag"] = true;
        REQUIRE(json["flag"].getNodeType() == JNodeType::boolean);
        REQUIRE(JNodeDataRef<JNodeBooleanData>(json.root()["flag"]).toString() == "true");
    }
}
TEST_CASE("JSON object array api.", "[JSON][Create][Object]")
{
    SECTION("Initialise JSON with JSON object passed to constructor.", "[JSON][Create][Array][Constructor]")
    {
        REQUIRE_NOTHROW(JSON{R"(["pi", 3.141 ])"});
    }
    SECTION("Initialise JSON with JSON array  passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
    {
        const JSON json{R"(["pi", 3.141 ])"};
        REQUIRE(json.root().getNodeType() == JNodeType::array);
        REQUIRE(json.root()[0].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()[0]).toString() == "pi");
        REQUIRE(json.root()[1].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[1]).toString() == "3.141");
    }
    SECTION("Initialise root JSON array with one entry containing a integer.", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 300;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "300");
    }
    SECTION("Initialise root JSON array with one entry containing a long.", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 30000l;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "30000");
    }
    SECTION("Initialise root JSON array with one entry containing a long long .", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 30000000ll;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "30000000");
    }
    SECTION("Initialise root JSON array with one entry containing a float.", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 3.141f;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "3.141");
    }
    SECTION("Initialise root JSON array with one entry containing a double.", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 3.141;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "3.141");
    }
    SECTION("Initialise root JSON array with one entry containing a long double.", "[JSON][Create][Number]")
    {
        JSON json;
        json[0] = 3.141L;
        REQUIRE(json[0].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()[0]).toString() == "3.141");
    }
    SECTION("Initialise root JSON array with one entry containing a const char *.", "[JSON][Create][String]")
    {
        JSON json;
        json[0] = "robert";
        REQUIRE(json[0].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()[0]).toString() == "robert");
    }
    SECTION("Initialise root JSON array with one entry containing a std::string.", "[JSON][Create][String]")
    {
        JSON json;
        json[0] = std::string{"robert"};
        REQUIRE(json[0].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()[0]).toString() == "robert");
    }
    SECTION("Initialise root JSON array with one entry containing a boolean.", "[JSON][Create][Boolean]")
    {
        JSON json;
        json[0] = true;
        REQUIRE(json[0].getNodeType() == JNodeType::boolean);
        REQUIRE(JNodeDataRef<JNodeBooleanData>(json.root()[0]).toString() == "true");
    }
}