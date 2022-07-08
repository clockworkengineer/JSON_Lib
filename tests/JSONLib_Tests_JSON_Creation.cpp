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
TEST_CASE("JSON object creation api.", "[JSON][Create]")
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
    SECTION("Initialise root JSON object with one entry containing a double.", "[JSON][Create][Number]")
    {
        JSON json;
        json["pi"] = 3.141;
        REQUIRE(json["pi"].getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>(json.root()["pi"]).toString() == "3.141");
    }
    SECTION("Initialise root JSON object with one entry containing a string.", "[JSON][Create][Number]")
    {
        JSON json;
        json["name"] = "robert";
        REQUIRE(json["name"].getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeStringData>(json.root()["name"]).toString() == "robert");
    }
}