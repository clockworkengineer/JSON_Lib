//
// Unit Tests: JSON
//
// Description: JSON parse collection types unit tests for JSON class
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
// ===========================
// Parse collection JSON types
// ===========================
TEST_CASE("JSON object for parse of collection types (array,object) ", "[JSON][Parse][Collection]")
{
    JSON json;
    SECTION("Parse an object ({\"name\":\"Robert\",\"Age\":15})", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"{\"name\":\"Robert\",\"Age\":15}"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::object);
    }
    SECTION("Parse an array ([ 777,9000,\"apples\"]) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"[777,9000,\"apples\"]"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::array);
    }
    SECTION("Parse an empty array ([]) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"[]"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::array);
    }
    SECTION("Parse an empty object ({}) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"{}"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::object);
    }
    SECTION("Parse an nested empty arrays ([[], [], []]) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"[[], [], []]"};
        REQUIRE_NOTHROW(json.parse(jsonSource));
    }
    SECTION("Parse an nested empty objects ([{}, {}, {}]) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"[[], [], []]"};
        REQUIRE_NOTHROW(json.parse(jsonSource));
    }
    SECTION("Parse an nested empty objects/arrays ([{}, [], {},   [], {}, []]) ", "[JSON][Parse][Collection]")
    {
        BufferSource jsonSource{"[{}, [], {},   [], {}, []]"};
        REQUIRE_NOTHROW(json.parse(jsonSource));
    }
}
// ========================================
// Parse collection JSON types and validate
// ========================================
TEST_CASE("JSON object for parse of collection types and check values", "[JSON][Parse][Collection][validate]")
{
    JSON json;
    SECTION("Parse an object {\"name\":\"Robert\",\"Age\":15} and check its value", "[JSON][Parse][Collection][validate]")
    {
        BufferSource jsonSource{"{\"Name\":\"Robert\",\"Age\":15}"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::object);
        REQUIRE(JNodeDataRef<JNodeObjectData>(json.root()).size() == 2);
        REQUIRE(JNodeDataRef<JNodeObjectData>(json.root()).contains("Name"));
        REQUIRE(JNodeDataRef<JNodeObjectData>(json.root()).contains("Age"));
        REQUIRE(JNodeDataRef<JNodeStringData>((json.root())["Name"]).getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())["Age"]).getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeStringData>((json.root())["Name"]).string() == "Robert");
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())["Age"]).toString() == "15");
    }
    SECTION("Parse an array [777,9000,\"apples\"] and check its value", "[JSON][Parse][Collection][validate]")
    {
        BufferSource jsonSource{"[777,9000,\"apples\"]"};
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::array);
        REQUIRE(JNodeDataRef<JNodeArrayData>(json.root()).size() == 3);
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())[0]).getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())[1]).getNodeType() == JNodeType::number);
        REQUIRE(JNodeDataRef<JNodeStringData>((json.root())[2]).getNodeType() == JNodeType::string);
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())[0]).toString() == "777");
        REQUIRE(JNodeDataRef<JNodeNumberData>((json.root())[1]).toString() == "9000");
        REQUIRE(JNodeDataRef<JNodeStringData>((json.root())[2]).string() == "apples");
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} and check its value", "[JSON][Parse][Collection][validate]")
    {
        BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
    SECTION("Parse an array [\"Dog\",1964,true,null ] and check its value", "[JSON][Parse][Collection][validate]")
    {
        BufferSource jsonSource{"[\"Dog\",1964,true,null]"};
        json.parse(jsonSource);
        checkArray(json.root());
     }
}

