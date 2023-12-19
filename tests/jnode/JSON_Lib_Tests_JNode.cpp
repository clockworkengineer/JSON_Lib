//
// Unit Tests: JSON_Lib_Tests_JNode
//
// Description: JNode access/index/reference/constructor/assignment
// unit tests for JSON class using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// ==================
// JNode constructors
// ==================
TEST_CASE("Check use of JNode constructors.", "[JSON][JNode][Constructor]")
{
  SECTION("Construct JNode(integer).", "[JSON][JNode][Constructor][Integer]")
  {
    JNode jNode(999);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 999);
  }
  SECTION("Construct JNode(long).", "[JSON][JNode][Constructor][Long]")
  {
    JNode jNode(99988899l);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 99988899l);
  }
  SECTION("Construct JNode(long long).", "[JSON][JNode][Constructor][Long Long]")
  {
    JNode jNode(99988899ll);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long long>() == 99988899ll);
  }
  SECTION("Construct JNode(float).", "[JSON][JNode][Constructor][Float]")
  {
    JNode jNode(777.999f);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 777.999f, 0.0001));
  }
  SECTION("Construct JNode(double).", "[JSON][JNode][Constructor][Double]")
  {
    JNode jNode(66666.8888);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 66666.8888, 0.0001));
  }
  SECTION("Construct JNode(long double).", "[JSON][JNode][Constructor][Long Double]")
  {
    JNode jNode(66666.8888l);
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 66666.8888l, 0.0001));
  }
  SECTION("Construct JNode(C string).", "[JSON][JNode][Constructor][CString]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Construct JNode(string).", "[JSON][JNode][Constructor][String]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Construct JNode(boolean).", "[JSON][JNode][Constructor][Boolean]")
  {
    JNode jNode(false);
    REQUIRE_FALSE(!jNode.isBoolean());
    REQUIRE_FALSE(JRef<Boolean>(jNode).value());
  }
  SECTION("Construct JNode(null).", "[JSON][JNode][Constructor][Null]")
  {
    JNode jNode(nullptr);
    REQUIRE_FALSE(!jNode.isNull());
    REQUIRE(JRef<Null>(jNode).getNull() == nullptr);
  }
  SECTION("Construct JNode(array).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4 };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
  }
  SECTION("Construct JNode(object).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
  }
  SECTION("Construct JNode(array with nested array).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4, JNode{ 5, 6, 7, 8 } };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
    REQUIRE(JRef<Number>(array[4][0]).value<int>() == 5);
    REQUIRE(JRef<Number>(array[4][1]).value<int>() == 6);
    REQUIRE(JRef<Number>(array[4][2]).value<int>() == 7);
    REQUIRE(JRef<Number>(array[4][3]).value<int>() == 8);
  }
  SECTION("Construct JNode(object with nested object).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 }, { "key3", JNode{ { "key4", 5555 }, { "key5", 7777 } } } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  }
  SECTION("Construct JNode(array with nested object).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4, JNode{ { "key4", 5555 }, { "key5", 7777 } } };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
    REQUIRE(JRef<Number>(array[4]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(array[4]["key5"]).value<int>() == 7777);
  }
  SECTION("Construct JNode(object with nested array).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 }, { "key3", JNode{ 5, 6, 7, 8 } } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"][0]).value<int>() == 5);
    REQUIRE(JRef<Number>(object["key3"][1]).value<int>() == 6);
    REQUIRE(JRef<Number>(object["key3"][2]).value<int>() == 7);
    REQUIRE(JRef<Number>(object["key3"][3]).value<int>() == 8);
  }
}
// =================
// JNode assignments
// =================
TEST_CASE("Check use of JNode assigment operators.", "[JSON][JNode][Assignment]")
{
  JNode jNode;
  SECTION("Assign integer to JNode.", "[JSON][JNode][Assignment][Integer]")
  {
    jNode = 999;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 999);
  }
  SECTION("Assign long to JNode.", "[JSON][JNode][Assignment][Long]")
  {
    jNode = 99988899l;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 99988899l);
  }
  SECTION("Assign long long to JNode.", "[JSON][JNode][Assignment][Long Long]")
  {
    jNode = 99988899ll;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long long>() == 99988899ll);
  }
  SECTION("Assign float to JNode.", "[JSON][JNode][Assignment][Float]")
  {
    jNode = 777.999f;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 777.999f, 0.0001));
  }
  SECTION("Assign double to JNode.", "[JSON][JNode][Assignment][Double]")
  {
    jNode = 66666.8888;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 66666.8888, 0.0001));
  }
  SECTION("Assign long double to JNode.", "[JSON][JNode][Assignment][Long Double]")
  {
    jNode = 66666.8888l;
    REQUIRE_FALSE(!jNode.isNumber());
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 66666.8888l, 0.0001));
  }
  SECTION("Assign C string to JNode.", "[JSON][JNode][Assignment][CString]")
  {
    jNode = "test string";
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Assign string to JNode.", "[JSON][JNode][Assignment][String]")
  {
    jNode = std::string("test string");
    REQUIRE_FALSE(!jNode.isString());
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Assign boolean to JNode.", "[JSON][JNode][Assignment][Boolean]")
  {
    jNode = false;
    REQUIRE_FALSE(!jNode.isBoolean());
    REQUIRE_FALSE(JRef<Boolean>(jNode).value());
  }
  SECTION("Assign nullptr to JNode.", "[JSON][JNode][Assignment][Null]")
  {
    jNode = nullptr;
    REQUIRE_FALSE(!jNode.isNull());
    REQUIRE(JRef<Null>(jNode).getNull() == nullptr);
  }
  SECTION("Assign array to JNode.", "[JSON][JNode][Assignment][Array]")
  {
    jNode = { 1, 2, 3, 4 };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
  }
  SECTION("Assign object to JNode.", "[JSON][JNode][Assignment][Object]")
  {
    jNode = { { "key1", 55 }, { "key2", 26666 } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
  }
  SECTION("Assign JNode with array with nested array.", "[JSON][JNode][Constructor][Array]")
  {
    jNode = { 1, 2, 3, 4, JNode{ 5, 6, 7, 8 } };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
    REQUIRE(JRef<Number>(array[4][0]).value<int>() == 5);
    REQUIRE(JRef<Number>(array[4][1]).value<int>() == 6);
    REQUIRE(JRef<Number>(array[4][2]).value<int>() == 7);
    REQUIRE(JRef<Number>(array[4][3]).value<int>() == 8);
  }
  SECTION("Assign JNode with object with a nested object.", "[JSON][JNode][Constructor][Object]")
  {
    jNode = { { "key1", 55 }, { "key2", 26666 }, { "key3", JNode{ { "key4", 5555 }, { "key5", 7777 } } } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  }
  SECTION("Assign JNode with array with a nested object.", "[JSON][JNode][Constructor][Array]")
  {
    jNode = { 1, 2, 3, 4, JNode{ { "key4", 5555 }, { "key5", 7777 } } };
    REQUIRE_FALSE(!jNode.isArray());
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
    REQUIRE(JRef<Number>(array[4]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(array[4]["key5"]).value<int>() == 7777);
  }
  SECTION("Assign JNode with object with nested a array.", "[JSON][JNode][Constructor][Object]")
  {
    jNode = { { "key1", 55 }, { "key2", 26666 }, { "key3", JNode{ 5, 6, 7, 8 } } };
    REQUIRE_FALSE(!jNode.isObject());
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"][0]).value<int>() == 5);
    REQUIRE(JRef<Number>(object["key3"][1]).value<int>() == 6);
    REQUIRE(JRef<Number>(object["key3"][2]).value<int>() == 7);
    REQUIRE(JRef<Number>(object["key3"][3]).value<int>() == 8);
  }
}
// ==============
// JNode Indexing
// ==============
TEST_CASE("Check use of JNode indexing operators.", "[JSON][JNode][Index]")
{
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing.", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JRef<String>(json["City"]).value() == "Southampton");
    REQUIRE(JRef<Number>(json["Population"]).value<int>() == 500000);
  }
  SECTION("Parse array and check its components using indexing.", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json[0]).value<int>() == 777);
    REQUIRE(JRef<Number>(json[1]).value<int>() == 9000);
    REQUIRE(JRef<String>(json[2]).value() == "apples");
  }
  SECTION("Parse array with embedded dictionary and check its components using indexing.", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"([777,{"City":"Southampton","Population":500000},"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json[0]).value<int>() == 777);
    REQUIRE(JRef<String>(json[1]["City"]).value() == "Southampton");
    REQUIRE(JRef<Number>(json[1]["Population"]).value<int>() == 500000);
    REQUIRE(JRef<String>(json[2]).value() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception.", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].isObject(), JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].isObject(), "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse array and check an invalid index generates exception.", "[JSON][JNode][Index]")
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
    REQUIRE(JRef<Null>(json.root()).getNull() == nullptr);
  }
  SECTION("Array reference.", "[JSON][JNode][Reference][Array]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<String>(json[2]).value() == "apples");
  }
  SECTION("Object reference.", "[JSON][JNode][Reference][Object]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000 })" };
    json.parse(jsonSource);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>(json["City"]).value() == "Southampton");
  }
}
// ===================================
// JNode Reference function exceptions
// ===================================
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
    REQUIRE_THROWS_AS(JRef<Null>(json.root()).getNull(), JNode::Error);
    REQUIRE_THROWS_WITH(JRef<Null>(json.root()).getNull(), "JNode Error: JNode not a null.");
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