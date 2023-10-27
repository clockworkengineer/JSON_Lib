//
// Unit Tests: JSON_Lib_Tests_Parse_Simple
//
// Description: JSON parse simple types unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// =======================
// Parse simple JSON types
// =======================
TEST_CASE("Check JSON parsing of simple types (number,string,boolean,null).", "[JSON][Parse][Simple]")
{
  const JSON json;
  SECTION("Parse an string (example string).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isString());
  }
  SECTION("Parse an number (6767).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isNumber());
  }
  SECTION("Parse an boolean (true).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isBoolean());
  }
  SECTION("Parse an boolean (false).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isBoolean());
  }
  SECTION("Parse an null.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isNull());
  }
  SECTION("Parse an empty string.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "\"\"" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!json.root().isString());
  }
}
// ====================================
// Parse simple JSON types and validate
// ====================================
TEST_CASE("Check JSON parsing of simple types and check values.", "[JSON][Parse][Simple][Validate]")
{
  const JSON json;
  SECTION("Parse an string (example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).getString() == "example string");
  }
  SECTION("Parse an string (another example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("another example string")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).getString() == "another example string");
  }
  SECTION("Parse an number (6767) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).get<int>() == 6767);
  }
  SECTION("Parse an number (190000) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "190000" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).get<int>() == 190000);
  }
  SECTION("Parse an boolean (true) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE(JRef<Boolean>(json.root()).getBoolean() == true);
  }
  SECTION("Parse an boolean (false) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE(JRef<Boolean>(json.root()).getBoolean() == false);
  }
  SECTION("Parse an null and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE(JRef<Null>(json.root()).getNull() == nullptr);
  }
}