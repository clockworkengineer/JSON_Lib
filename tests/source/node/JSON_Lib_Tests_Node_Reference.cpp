#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check Node reference functions work.", "[JSON][Node][Reference]")
{
  const JSON json;
  SECTION("Number reference.", "[JSON][Node][Reference][Number]")
  {
    BufferSource jsonSource{ "45500" };
    json.parse(jsonSource);
    REQUIRE(NRef<Number>(json.root()).value<int>() == 45500);
  }
  SECTION("String reference.", "[JSON][Node][Reference][String]")
  {
    BufferSource jsonSource{ R"("0123456789")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "0123456789");
  }
  SECTION("Boolean reference.", "[JSON][Node][Reference][Boolean]")
  {
    BufferSource jsonSource{ R"(true)" };
    json.parse(jsonSource);
    REQUIRE(NRef<Boolean>(json.root()).value() == true);
  }
  SECTION("Null reference.", "[JSON][Node][Reference][Null]")
  {
    BufferSource jsonSource{ R"(null)" };
    json.parse(jsonSource);
    REQUIRE(NRef<Null>(json.root()).value() == nullptr);
  }
  SECTION("Array reference.", "[JSON][Node][Reference][Array]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(NRef<Array>(json.root()).size() == 3);
    REQUIRE(NRef<String>((json.root())[2]).value() == "apples");
  }
  SECTION("Object reference.", "[JSON][Node][Reference][Object]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE(NRef<Object>(json.root()).size() == 2);
    REQUIRE(NRef<String>((json.root())["City"]).value() == "Southampton");
  }
}

TEST_CASE("Check Node reference function exceptions.", "[JSON][Node][Reference][Exception]")
{
  const JSON json;
  SECTION("String reference an number.", "[JSON][Node][Reference][String]")
  {
    BufferSource jsonSource{ "45500" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<String>(json.root()).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<String>(json.root()).value(), "Node Error: Node not a string.");
  }
  SECTION("Number reference a string.", "[JSON][Node][Reference][Number][Exception]")
  {
    BufferSource jsonSource{ R"("0123456789")" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<Number>(json.root()).is<int>(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Number>(json.root()).is<int>(), "Node Error: Node not a number.");
  }
  SECTION("Null reference a boolean.", "[JSON][Node][Reference][Null][Exception]")
  {
    BufferSource jsonSource{ R"(true)" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<Null>(json.root()).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Null>(json.root()).value(), "Node Error: Node not a null.");
  }
  SECTION("Boolean reference a null.", "[JSON][Node][Reference][Boolean][Exception]")
  {
    BufferSource jsonSource{ R"(null)" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<Boolean>(json.root()).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Boolean>(json.root()).value(), "Node Error: Node not a boolean.");
  }
  SECTION("Object reference an array.", "[JSON][Node][Reference][Object][Exception]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<Object>(json.root()).size(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Object>(json.root()).size(), "Node Error: Node not an object.");
  }
  SECTION("Array reference an object.", "[JSON][Node][Reference][Array][Exception]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(NRef<Array>(json.root()).size(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Array>(json.root()).size(), "Node Error: Node not an array.");
  }
}