//
// Unit Tests: JSON_Lib_Tests_Parse_Collection
//
// Description: JSON parse collection types unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// ===========================
// Parse collection JSON types
// ===========================
TEST_CASE("Check JSON parsing of collection types (array,object).", "[JSON][Parse][Collection]")
{
  const JSON json;
  SECTION(R"(Parse an object ({"name":"Robert","Age":15}).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"({"name":"Robert","Age":15})" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isObject());
  }
  SECTION(R"(Parse an array ([777,9000,"apples"]).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isArray());
  }
  SECTION("Parse an empty array ([]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[]" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isArray());
  }
  SECTION("Parse an empty object ({}).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "{}" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isObject());
  }
  SECTION("Parse an nested empty arrays ([[], [], []]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[[], [], []]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse an nested empty objects ([{}, {}, {}]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[[], [], []]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse an nested empty objects/arrays ([{}, [], {},   [], {}, []]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[{}, [], {},   [], {}, []]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
}
// ========================================
// Parse collection JSON types and validate
// ========================================
TEST_CASE("Check JSON parsing of collection types and check values.", "[JSON][Parse][Collection][validate]")
{
  const JSON json;
  SECTION(R"(Parse an object {"name":"Robert","Age":15} and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"Name":"Robert","Age":15})" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isObject());
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<Object>(json.root()).contains("Name"));
    REQUIRE(JRef<Object>(json.root()).contains("Age"));
    REQUIRE_FALSE(!json.root()["Name"].isString());
    REQUIRE_FALSE(!json.root()["Age"].isNumber());
    REQUIRE(JRef<String>(json["Name"]).value() == "Robert");
    REQUIRE(JRef<Number>(json["Age"]).value<int>() == 15);
  }
  SECTION(R"(Parse an array [777,9000,"apples"] and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isArray());
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE_FALSE(!json.root()[0].isNumber());
    REQUIRE_FALSE(!json.root()[1].isNumber());
    REQUIRE_FALSE(!json.root()[2].isString());
    REQUIRE(JRef<Number>(json[0]).value<int>() == 777);
    REQUIRE(JRef<Number>(json[1]).value<int>() == 9000);
    REQUIRE(JRef<String>(json[2]).value() == "apples");
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} and check its value.)",
    "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  SECTION(R"(Parse an array ["Dog",1964,true,null ] and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"(["Dog",1964,true,null])" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
}
