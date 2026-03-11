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
    BufferSource jsonSource{ "[{}, {}, {}]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse an nested empty objects/arrays ([{}, [], {},   [], {}, []]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[{}, [], {},   [], {}, []]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Parse a single-element array ([42]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[42]" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Array>(json.root()));
  }
  SECTION(R"(Parse a single-key object ({"x":1}).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"({"x":1})" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Object>(json.root()));
  }
  SECTION("Parse deeply nested arrays ([[[1]]]).", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ "[[[1]]]" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION(R"(Parse an array of objects ([{"a":1},{"b":2}]).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"([{"a":1},{"b":2}])" };
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION(R"(Parse an object with an array value ({"items":[1,2,3]}).)", "[JSON][Parse][Collection]")
  {
    BufferSource jsonSource{ R"({"items":[1,2,3]})" };
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
    REQUIRE(NRef<Object>(json.root()).size() == 2);
    REQUIRE(NRef<Object>(json.root()).contains("Name"));
    REQUIRE(NRef<Object>(json.root()).contains("Age"));
    REQUIRE_FALSE(!isA<String>(json.root()["Name"]));
    REQUIRE_FALSE(!isA<Number>(json.root()["Age"]));
    REQUIRE(NRef<String>((json.root())["Name"]).value() == "Robert");
    REQUIRE(NRef<Number>((json.root())["Age"]).value<int>() == 15);
  }
  SECTION(R"(Parse an array [777,9000,"apples"] and check its value.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Array>(json.root()));
    REQUIRE(NRef<Array>(json.root()).size() == 3);
    REQUIRE_FALSE(!isA<Number>(json.root()[0]));
    REQUIRE_FALSE(!isA<Number>(json.root()[1]));
    REQUIRE_FALSE(!isA<String>(json.root()[2]));
    REQUIRE(NRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(NRef<Number>((json.root())[1]).value<int>() == 9000);
    REQUIRE(NRef<String>((json.root())[2]).value() == "apples");
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
  SECTION(R"(Parse nested object {"person":{"name":"Alice","age":30}} and check its values.)",
    "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"person":{"name":"Alice","age":30}})" };
    json.parse(jsonSource);
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(NRef<Object>(json.root()).size() == 1);
    REQUIRE(NRef<Object>(json.root()).contains("person"));
    REQUIRE(isA<Object>(json.root()["person"]));
    REQUIRE(NRef<String>(json.root()["person"]["name"]).value() == "Alice");
    REQUIRE(NRef<Number>(json.root()["person"]["age"]).value<int>() == 30);
  }
  SECTION("Parse nested array [[1,2],[3,4]] and check its values.", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"([[1,2],[3,4]])" };
    json.parse(jsonSource);
    REQUIRE(isA<Array>(json.root()));
    REQUIRE(NRef<Array>(json.root()).size() == 2);
    REQUIRE(NRef<Number>(json.root()[0][0]).value<int>() == 1);
    REQUIRE(NRef<Number>(json.root()[0][1]).value<int>() == 2);
    REQUIRE(NRef<Number>(json.root()[1][0]).value<int>() == 3);
    REQUIRE(NRef<Number>(json.root()[1][1]).value<int>() == 4);
  }
  SECTION(R"(Parse {"active":true,"deleted":false,"ref":null} and check boolean and null values.)",
    "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"active":true,"deleted":false,"ref":null})" };
    json.parse(jsonSource);
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(NRef<Boolean>(json.root()["active"]).value() == true);
    REQUIRE(NRef<Boolean>(json.root()["deleted"]).value() == false);
    REQUIRE(isA<Null>(json.root()["ref"]));
  }
  SECTION(R"(Parse {"tags":["a","b","c"]} and check inner array values.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"({"tags":["a","b","c"]})" };
    json.parse(jsonSource);
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(isA<Array>(json.root()["tags"]));
    REQUIRE(NRef<Array>(json.root()["tags"]).size() == 3);
    REQUIRE(NRef<String>(json.root()["tags"][0]).value() == "a");
    REQUIRE(NRef<String>(json.root()["tags"][1]).value() == "b");
    REQUIRE(NRef<String>(json.root()["tags"][2]).value() == "c");
  }
  SECTION(R"(Parse [{"id":1},{"id":2}] and check inner object values.)", "[JSON][Parse][Collection][validate]")
  {
    BufferSource jsonSource{ R"([{"id":1},{"id":2}])" };
    json.parse(jsonSource);
    REQUIRE(isA<Array>(json.root()));
    REQUIRE(NRef<Array>(json.root()).size() == 2);
    REQUIRE(isA<Object>(json.root()[0]));
    REQUIRE(isA<Object>(json.root()[1]));
    REQUIRE(NRef<Number>(json.root()[0]["id"]).value<int>() == 1);
    REQUIRE(NRef<Number>(json.root()[1]["id"]).value<int>() == 2);
  }
}
