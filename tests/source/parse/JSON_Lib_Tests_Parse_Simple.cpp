#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON parsing of simple types (number,string,boolean,null).", "[JSON][Parse][Simple]")
{
  const JSON json;
  SECTION("Parse an string (example string).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<String>(json.root()));
  }
  SECTION("Parse an number (6767).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Number>(json.root()));
  }
  SECTION("Parse an boolean (true).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Boolean>(json.root()));
  }
  SECTION("Parse an boolean (false).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Boolean>(json.root()));
  }
  SECTION("Parse an null.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Null>(json.root()));
  }
  SECTION("Parse an empty string.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "\"\"" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<String>(json.root()));
  }
}
TEST_CASE("Check JSON parsing of simple types and check values.", "[JSON][Parse][Simple][Validate]")
{
  const JSON json;
  SECTION("Parse an string (example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "example string");
  }
  SECTION("Parse an string (another example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("another example string")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "another example string");
  }
  SECTION("Parse an number (6767) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).value<int>() == 6767);
  }
  SECTION("Parse an number (190000) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "190000" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).value<int>() == 190000);
  }
  SECTION("Parse an boolean (true) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE(JRef<Boolean>(json.root()).value() == true);
  }
  SECTION("Parse an boolean (false) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE(JRef<Boolean>(json.root()).value() == false);
  }
  SECTION("Parse an null and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE(JRef<Null>(json.root()).value() == nullptr);
  }
}