#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]")
{
  const JSON json;
  SECTION("Number reference.", "[JSON][JNode][Reference][Number]")
  {
    BufferSource jsonSource{ "45500" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).value<int>() == 45500);
  }
  SECTION("String reference.", "[JSON][JNode][Reference][String]")
  {
    BufferSource jsonSource{ R"("0123456789")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "0123456789");
  }
  SECTION("Boolean reference.", "[JSON][JNode][Reference][Boolean]")
  {
    BufferSource jsonSource{ R"(true)" };
    json.parse(jsonSource);
    REQUIRE(JRef<Boolean>(json.root()).value() == true);
  }
  SECTION("Null reference.", "[JSON][JNode][Reference][Null]")
  {
    BufferSource jsonSource{ R"(null)" };
    json.parse(jsonSource);
    REQUIRE(JRef<Null>(json.root()).value() == nullptr);
  }
  SECTION("Array reference.", "[JSON][JNode][Reference][Array]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<String>((json.root())[2]).value() == "apples");
  }
  SECTION("Object reference.", "[JSON][JNode][Reference][Object]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).value() == "Southampton");
  }
}

TEST_CASE("Check JNode reference function exceptions.", "[JSON][JNode][Reference][Exception]")
{
  const JSON json;
  SECTION("String reference an number.", "[JSON][JNode][Reference][String]")
  {
    BufferSource jsonSource{ "45500" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<String>(json.root()).value(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<String>(json.root()).value(), "JNode Error: JNode not a string.");
  }
  SECTION("Number reference a string.", "[JSON][JNode][Reference][Number][Exception]")
  {
    BufferSource jsonSource{ R"("0123456789")" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<Number>(json.root()).is<int>(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Number>(json.root()).is<int>(), "JNode Error: JNode not a number.");
  }
  SECTION("Null reference a boolean.", "[JSON][JNode][Reference][Null][Exception]")
  {
    BufferSource jsonSource{ R"(true)" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<Null>(json.root()).value(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Null>(json.root()).value(), "JNode Error: JNode not a null.");
  }
  SECTION("Boolean reference a null.", "[JSON][JNode][Reference][Boolean][Exception]")
  {
    BufferSource jsonSource{ R"(null)" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<Boolean>(json.root()).value(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Boolean>(json.root()).value(), "JNode Error: JNode not a boolean.");
  }
  SECTION("Object reference an array.", "[JSON][JNode][Reference][Object][Exception]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<Object>(json.root()).size(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Object>(json.root()).size(), "JNode Error: JNode not an object.");
  }
  SECTION("Array reference an object.", "[JSON][JNode][Reference][Array][Exception]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(JRef<Array>(json.root()).size(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Array>(json.root()).size(), "JNode Error: JNode not an array.");
  }
}