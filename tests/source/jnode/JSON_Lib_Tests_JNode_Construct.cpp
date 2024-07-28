#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check use of JNode constructors.", "[JSON][JNode][Constructor]")
{
  SECTION("Construct JNode(integer).", "[JSON][JNode][Constructor][Integer]")
  {
    JNode jNode(999);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 999);
  }
  SECTION("Construct JNode(long).", "[JSON][JNode][Constructor][Long]")
  {
    JNode jNode(99988899l);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 99988899l);
  }
  SECTION("Construct JNode(long long).", "[JSON][JNode][Constructor][Long Long]")
  {
    JNode jNode(99988899ll);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long long>() == 99988899ll);
  }
  SECTION("Construct JNode(float).", "[JSON][JNode][Constructor][Float]")
  {
    JNode jNode(777.999f);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 777.999f, 0.0001));
  }
  SECTION("Construct JNode(double).", "[JSON][JNode][Constructor][Double]")
  {
    JNode jNode(66666.8888);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 66666.8888, 0.0001));
  }
  SECTION("Construct JNode(long double).", "[JSON][JNode][Constructor][Long Double]")
  {
    JNode jNode(66666.8888l);
    REQUIRE_FALSE(!isA<Number>(jNode));
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 66666.8888l, 0.0001));
  }
  SECTION("Construct JNode(C string).", "[JSON][JNode][Constructor][CString]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!isA<String>(jNode));
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Construct JNode(string).", "[JSON][JNode][Constructor][String]")
  {
    JNode jNode("test string");
    REQUIRE_FALSE(!isA<String>(jNode));
    REQUIRE(JRef<String>(jNode).value() == "test string");
  }
  SECTION("Construct JNode(boolean).", "[JSON][JNode][Constructor][Boolean]")
  {
    JNode jNode(false);
    REQUIRE_FALSE(!isA<Boolean>(jNode));
    REQUIRE_FALSE(JRef<Boolean>(jNode).value());
  }
  SECTION("Construct JNode(null).", "[JSON][JNode][Constructor][Null]")
  {
    JNode jNode(nullptr);
    REQUIRE_FALSE(!isA<Null>(jNode));
    REQUIRE(JRef<Null>(jNode).value() == nullptr);
  }
  SECTION("Construct JNode(array).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4 };
    REQUIRE_FALSE(!isA<Array>(jNode));
    auto &array = JRef<Array>(jNode).value();
    REQUIRE(JRef<Number>(array[0]).value<int>() == 1);
    REQUIRE(JRef<Number>(array[1]).value<int>() == 2);
    REQUIRE(JRef<Number>(array[2]).value<int>() == 3);
    REQUIRE(JRef<Number>(array[3]).value<int>() == 4);
  }
  SECTION("Construct JNode(object).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 } };
    REQUIRE_FALSE(!isA<Object>(jNode));
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
  }
  SECTION("Construct JNode(array with nested array).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4, JNode{ 5, 6, 7, 8 } };
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
  SECTION("Construct JNode(object with nested object).", "[JSON][JNode][Constructor][Object]")
  {
    JNode jNode{ { "key1", 55 }, { "key2", 26666 }, { "key3", JNode{ { "key4", 5555 }, { "key5", 7777 } } } };
    REQUIRE_FALSE(!isA<Object>(jNode));
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).value<int>() == 55);
    REQUIRE(JRef<Number>(object["key2"]).value<int>() == 26666);
    REQUIRE(JRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
    REQUIRE(JRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  }
  SECTION("Construct JNode(array with nested object).", "[JSON][JNode][Constructor][Array]")
  {
    JNode jNode{ 1, 2, 3, 4, JNode{ { "key4", 5555 }, { "key5", 7777 } } };
    REQUIRE_FALSE(!isA<Array>(jNode));
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
