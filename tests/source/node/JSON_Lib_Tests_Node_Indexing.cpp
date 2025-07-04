#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check use of Node indexing operators.", "[JSON][Node][Index]")
{
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JRef<String>((json.root())["City"]).value() == "Southampton");
    REQUIRE(JRef<Number>((json.root())["Population"]).value<int>() == 500000);
  }
  SECTION("Parse array and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(JRef<Number>((json.root())[1]).value<int>() == 9000);
    REQUIRE(JRef<String>((json.root())[2]).value() == "apples");
  }
  SECTION("Parse array with embedded dictionary and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"([777,{"City":"Southampton","Population":500000},"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(JRef<String>((json.root())[1]["City"]).value() == "Southampton");
    REQUIRE(JRef<Number>((json.root())[1]["Population"]).value<int>() == 500000);
    REQUIRE(JRef<String>((json.root())[2]).value() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(isA<Object>(json.root()["Cityy"]), Node::Error);
    REQUIRE_THROWS_WITH(isA<Object>(json.root()["Cityy"]), "Node Error: Invalid key used to access object.");
  }
  SECTION("Parse array and check an invalid index generates exception.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS(isA<Array>(json.root()[3]), Node::Error);
    REQUIRE_THROWS_WITH(isA<Array>(json.root()[3]), "Node Error: Invalid index used to access array.");
  }
}