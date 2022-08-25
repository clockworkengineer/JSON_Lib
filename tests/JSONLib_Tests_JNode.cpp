//
// Unit Tests: JSON
//
// Description: JNode access/index/reference/constructor unit tests for JSON class
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
// ==================
// JNode constructors
// ==================
TEST_CASE("Use of JNode constructors", "[JSON][JNode][Constructor]")
{
  SECTION("Test JNode(integer).", "[JSON][JNode][Constructor][Integer]")
  {
    JNode jNode(999);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isInt());
    REQUIRE(JRef<Number>(jNode).getNumber().getInt() == 999);
  }
  SECTION("Test JNode(long).", "[JSON][JNode][Constructor][Long]")
  {
    JNode jNode(99988899l);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isLong());
    REQUIRE(JRef<Number>(jNode).getNumber().getInt() == 99988899l);
  }
  SECTION("Test JNode(float).", "[JSON][JNode][Constructor][Float]")
  {
    JNode jNode(777.999f);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).getNumber().getFloat(), 777.999f, 0.0001));
  }
  SECTION("Test JNode(double).", "[JSON][JNode][Constructor][Double]")
  {
    JNode jNode(66666.8888);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).getNumber().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).getNumber().getDouble(), 66666.8888, 0.0001));
  }
  SECTION("Test JNode(C string).", "[JSON][JNode][Constructor][CString]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).getString() == "test string");
  }
  SECTION("Test JNode(string).", "[JSON][JNode][Constructor]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).getString() == "test string");
  }
  SECTION("Test JNode(boolean).", "[JSON][JNode][Constructor][String]")
  {
    JNode jNode(false);
    REQUIRE_FALSE(!jNode.isBoolean());
    REQUIRE_FALSE(JRef<Boolean>(jNode).getBoolean());
  }
  SECTION("Test JNode(null).", "[JSON][JNode][Constructor][Null]")
  {
    JNode jNode(nullptr);
    REQUIRE_FALSE(!jNode.isNull());
    REQUIRE(JRef<Null>(jNode).getNull() == nullptr);
  }
  SECTION("Test JNode(array).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4 };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).getArrayEntries();
    REQUIRE(JRef<Number>(array[0]).getNumber().getInt() == 1);
    REQUIRE(JRef<Number>(array[1]).getNumber().getInt() == 2);
    REQUIRE(JRef<Number>(array[2]).getNumber().getInt() == 3);
    REQUIRE(JRef<Number>(array[3]).getNumber().getInt() == 4);
  }
  SECTION("Test JNode(object).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).getNumber().getInt() == 55);
    REQUIRE(JRef<Number>(object["key2"]).getNumber().getInt() == 26666);
  }
}
// ==============
// JNode Indexing
// ==============
TEST_CASE("Use of JNode indexing operators", "[JSON][JNode][Index]")
{
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JRef<String>((json.root())["City"]).getString() == "Southampton");
    REQUIRE(JRef<Number>((json.root())["Population"]).getNumber().getInt() == 500000);
  }
  SECTION("Parse list and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).getNumber().getInt() == 777);
    REQUIRE(JRef<Number>((json.root())[1]).getNumber().getInt() == 9000);
    REQUIRE(JRef<String>((json.root())[2]).getString() == "apples");
  }
  SECTION(
    "Parse list with embedded dictionary and check its components using "
    "indexing",
    "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"([777,{"City":"Southampton","Population":500000},"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).getNumber().getInt() == 777);
    REQUIRE(JRef<String>((json.root())[1]["City"]).getString() == "Southampton");
    REQUIRE(JRef<Number>((json.root())[1]["Population"]).getNumber().getInt() == 500000);
    REQUIRE(JRef<String>((json.root())[2]).getString() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].isObject(), JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].isObject(), "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())[3].isArray(), JNode::Error);
    REQUIRE_THROWS_WITH((json.root())[3].isArray(), "JNode Error: Invalid index used to access array.");
  }
}
// =========================
// JNode Reference functions
// =========================
TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]")
{
  const JSON json;
  SECTION("Integer reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{ "45500" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).getNumber().getInt() == 45500);
  }
  SECTION("String reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{ R"("0123456789")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).getString() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<String>((json.root())[2]).getString() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).getString() == "Southampton");
  }
}