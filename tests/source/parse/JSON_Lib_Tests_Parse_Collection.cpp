#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON parsing of collection types (array,object).", "[JSON][Parse][Collection]")
{
  const JSON json;
  SECTION(R"(Parse an object ({"name":"Robert","Age":15}).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"({"name":"Robert","Age":15})" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Object>(json.root()));
  }
  SECTION(R"(Parse an array ([777,9000,"apples"]).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Array>(json.root()));
  }
  SECTION("Parse an empty array ([]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[]" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Array>(json.root()));
  }
  SECTION("Parse an empty object ({}).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "{}" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Object>(json.root()));
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

TEST_CASE("Check JSON parsing of collection types and check values.", "[JSON][Parse][Collection][validate]")
{
  const JSON json;
  SECTION(R"(Parse an object {"name":"Robert","Age":15} and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"Name":"Robert","Age":15})" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Object>(json.root()));
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<Object>(json.root()).contains("Name"));
    REQUIRE(JRef<Object>(json.root()).contains("Age"));
    REQUIRE_FALSE(!isA<String>(json.root()["Name"]));
    REQUIRE_FALSE(!isA<Number>(json.root()["Age"]));
    REQUIRE(JRef<String>((json.root())["Name"]).value() == "Robert");
    REQUIRE(JRef<Number>((json.root())["Age"]).value<int>() == 15);
  }
  SECTION(R"(Parse an array [777,9000,"apples"] and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Array>(json.root()));
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE_FALSE(!isA<Number>(json.root()[0]));
    REQUIRE_FALSE(!isA<Number>(json.root()[1]));
    REQUIRE_FALSE(!isA<String>(json.root()[2]));
    REQUIRE(JRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(JRef<Number>((json.root())[1]).value<int>() == 9000);
    REQUIRE(JRef<String>((json.root())[2]).value() == "apples");
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
