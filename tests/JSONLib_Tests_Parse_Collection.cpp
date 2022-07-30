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
TEST_CASE("JSON object for parse of collection types (array,object) ",
          "[JSON][Parse][Collection]") {
  const JSON json;
  SECTION(R"(Parse an object ({"name":"Robert","Age":15}))",
          "[JSON][Parse][Collection]") {
    BufferSource jsonSource{R"({"name":"Robert","Age":15})"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::object);
  }
  SECTION(R"(Parse an array ([ 777,9000,"apples"]) )",
          "[JSON][Parse][Collection]") {
    BufferSource jsonSource{R"([777,9000,"apples"])"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::array);
  }
  SECTION("Parse an empty array ([]) ", "[JSON][Parse][Collection]") {
    BufferSource jsonSource{"[]"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::array);
  }
  SECTION("Parse an empty object ({}) ", "[JSON][Parse][Collection]") {
    BufferSource jsonSource{"{}"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::object);
  }
  SECTION("Parse an nested empty arrays ([[], [], []]) ",
          "[JSON][Parse][Collection]") {
    BufferSource jsonSource{"[[], [], []]"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse an nested empty objects ([{}, {}, {}]) ",
          "[JSON][Parse][Collection]") {
    BufferSource jsonSource{"[[], [], []]"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse an nested empty objects/arrays ([{}, [], {},   [], {}, []]) ",
          "[JSON][Parse][Collection]") {
    BufferSource jsonSource{"[{}, [], {},   [], {}, []]"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
}
// ========================================
// Parse collection JSON types and validate
// ========================================
TEST_CASE("JSON object for parse of collection types and check values",
          "[JSON][Parse][Collection][validate]") {
  const JSON json;
  SECTION(
      R"(Parse an object {"name":"Robert","Age":15} and check its value)",
      "[JSON][Parse][Collection][validate]") {
    BufferSource jsonSource{R"({"Name":"Robert","Age":15})"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::object);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<Object>(json.root()).contains("Name"));
    REQUIRE(JRef<Object>(json.root()).contains("Age"));
    REQUIRE(JRef<String>((json.root())["Name"]).getType() ==
            JNodeType::string);
    REQUIRE(JRef<Number>((json.root())["Age"]).getType() ==
            JNodeType::number);
    REQUIRE(JRef<String>((json.root())["Name"]).string() == "Robert");
    REQUIRE(JRef<Number>((json.root())["Age"]).toString() == "15");
  }
  SECTION(R"(Parse an array [777,9000,"apples"] and check its value)",
          "[JSON][Parse][Collection][validate]") {
    BufferSource jsonSource{R"([777,9000,"apples"])"};
    json.parse(jsonSource);
    REQUIRE((json.root()).getType() == JNodeType::array);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<Number>((json.root())[0]).getType() ==
            JNodeType::number);
    REQUIRE(JRef<Number>((json.root())[1]).getType() ==
            JNodeType::number);
    REQUIRE(JRef<String>((json.root())[2]).getType() ==
            JNodeType::string);
    REQUIRE(JRef<Number>((json.root())[0]).toString() == "777");
    REQUIRE(JRef<Number>((json.root())[1]).toString() == "9000");
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} and )"
          "check its value",
          "[JSON][Parse][Collection][validate]") {
    BufferSource jsonSource{R"({"City":"Southampton","Population":500000})"};
    json.parse(jsonSource);
    checkObject(json.root());
  }
  SECTION(R"(Parse an array ["Dog",1964,true,null ] and check its value)",
          "[JSON][Parse][Collection][validate]") {
    BufferSource jsonSource{R"(["Dog",1964,true,null])"};
    json.parse(jsonSource);
    checkArray(json.root());
  }
}
