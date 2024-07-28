#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check use of JNode assigment operators.", "[JSON][JNode][Assignment]")
{
  JNode jNode;
  SECTION("Assign integer to JNode.", "[JSON][JNode][Assignment][Integer]")
  {
    jNode = 999;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 999);
  }
  SECTION("Assign long to JNode.", "[JSON][JNode][Assignment][Long]")
  {
    jNode = 99988899l;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 99988899l);
  }
  SECTION("Assign long long to JNode.", "[JSON][JNode][Assignment][Long Long]")
  {
    jNode = 99988899ll;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long long>() == 99988899ll);
  }
  SECTION("Assign float to JNode.", "[JSON][JNode][Assignment][Float]")
  {
    jNode = 777.999f;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 777.999f, 0.0001));
  }
  SECTION("Assign double to JNode.", "[JSON][JNode][Assignment][Double]")
  {
    jNode = 66666.8888;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 66666.8888, 0.0001));
  }
  SECTION("Assign long double to JNode.", "[JSON][JNode][Assignment][Long Double]")
  {
    jNode = 66666.8888l;
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 66666.8888l, 0.0001));
  }
  SECTION("Assign C string to JNode.", "[JSON][JNode][Assignment][CString]")
  {
    jNode = "test string";
    REQUIRE_FALSE(!isA<String>(jNode));
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Assign string to JNode.", "[JSON][JNode][Assignment][String]")
  {
    jNode = std::string("test string");
    REQUIRE_FALSE(!isA<String>(jNode));
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Assign boolean to JNode.", "[JSON][JNode][Assignment][Boolean]")
  {
    jNode = false;
    REQUIRE_FALSE(!isA<Boolean>(jNode));
    REQUIRE_FALSE(JRef<Boolean>(jNode).value());
  }
  SECTION("Assign nullptr to JNode.", "[JSON][JNode][Assignment][Null]")
  {
    jNode = nullptr;
    REQUIRE_FALSE(!isA<Null>(jNode));
    REQUIRE(JRef<Null>(jNode).value() == nullptr);
  }
  SECTION("Assign array to JNode.", "[JSON][JNode][Assignment][Array]")
  {
    jNode = { 1, 2, 3, 4 };
    REQUIRE_FALSE(!isA<Array>(jNode));
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
  }
  SECTION("Assign object to JNode.", "[JSON][JNode][Assignment][Object]")
  {
    jNode = { { "key1", 55 }, { "key2", 26666 } };
    REQUIRE_FALSE(!isA<Object>(jNode));
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
  }
  SECTION("Assign JNode with array with nested array.", "[JSON][JNode][Constructor][Array]")
  {
    jNode = { 1, 2, 3, 4, JNode{ 5, 6, 7, 8 } };
    REQUIRE_FALSE(!isA<Array>(jNode));
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
    REQUIRE_FALSE(!isA<Object>(jNode));
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  }
  SECTION("Assign JNode with array with a nested object.", "[JSON][JNode][Constructor][Array]")
  {
    jNode = { 1, 2, 3, 4, JNode{ { "key4", 5555 }, { "key5", 7777 } } };
    REQUIRE_FALSE(!isA<Array>(jNode));
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
    REQUIRE_FALSE(!isA<Object>(jNode));
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"][0]).value<int>() == 5);
    REQUIRE(JRef<Number>(object["key3"][1]).value<int>() == 6);
    REQUIRE(JRef<Number>(object["key3"][2]).value<int>() == 7);
    REQUIRE(JRef<Number>(object["key3"][3]).value<int>() == 8);
  }
}