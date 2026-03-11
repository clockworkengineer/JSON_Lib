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

TEST_CASE("Check NRef on directly constructed nodes.", "[JSON][Node][Reference]")
{
  SECTION("NRef<Number> on constructed integer node.", "[JSON][Node][Reference][Number]")
  {
    Node jNode(42);
    REQUIRE(NRef<Number>(jNode).value<int>() == 42);
  }
  SECTION("NRef<String> on constructed string node.", "[JSON][Node][Reference][String]")
  {
    Node jNode("hello");
    REQUIRE(NRef<String>(jNode).value() == "hello");
  }
  SECTION("NRef<Boolean> on constructed true node.", "[JSON][Node][Reference][Boolean]")
  {
    Node jNode(true);
    REQUIRE(NRef<Boolean>(jNode).value() == true);
  }
  SECTION("NRef<Boolean> on constructed false node.", "[JSON][Node][Reference][Boolean]")
  {
    Node jNode(false);
    REQUIRE(NRef<Boolean>(jNode).value() == false);
  }
  SECTION("NRef<Null> on constructed null node.", "[JSON][Node][Reference][Null]")
  {
    Node jNode(nullptr);
    REQUIRE(NRef<Null>(jNode).value() == nullptr);
  }
  SECTION("NRef<Hole> on a Hole node.", "[JSON][Node][Reference]")
  {
    Node jNode = Node::make<Hole>();
    REQUIRE_NOTHROW(NRef<Hole>(jNode));
  }
  SECTION("NRef on nested array element.", "[JSON][Node][Reference][Array]")
  {
    Node jNode{ 10, 20, 30 };
    REQUIRE(NRef<Number>(jNode[0]).value<int>() == 10);
    REQUIRE(NRef<Number>(jNode[1]).value<int>() == 20);
    REQUIRE(NRef<Number>(jNode[2]).value<int>() == 30);
  }
  SECTION("NRef on nested object entry.", "[JSON][Node][Reference][Object]")
  {
    Node jNode{ { "key", 99 } };
    REQUIRE(NRef<Number>(jNode["key"]).value<int>() == 99);
  }
  SECTION("NRef on ObjectEntry overload.", "[JSON][Node][Reference][Object]")
  {
    Node jNode{ { "name", Node("Alice") }, { "age", Node(30) } };
    auto &entries = NRef<Object>(jNode).value();
    REQUIRE(NRef<String>(entries[0]).value() == "Alice");
    REQUIRE(NRef<Number>(entries[1]).value<int>() == 30);
  }
}

TEST_CASE("Check isA<T> type predicate.", "[JSON][Node][Reference]")
{
  SECTION("isA<Number> matches a number node.", "[JSON][Node][Reference]")
  {
    Node jNode(1);
    REQUIRE(isA<Number>(jNode));
    REQUIRE_FALSE(isA<String>(jNode));
    REQUIRE_FALSE(isA<Boolean>(jNode));
    REQUIRE_FALSE(isA<Null>(jNode));
    REQUIRE_FALSE(isA<Array>(jNode));
    REQUIRE_FALSE(isA<Object>(jNode));
  }
  SECTION("isA<String> matches a string node.", "[JSON][Node][Reference]")
  {
    Node jNode("test");
    REQUIRE(isA<String>(jNode));
    REQUIRE_FALSE(isA<Number>(jNode));
  }
  SECTION("isA<Boolean> matches a boolean node.", "[JSON][Node][Reference]")
  {
    Node jNode(true);
    REQUIRE(isA<Boolean>(jNode));
    REQUIRE_FALSE(isA<Null>(jNode));
  }
  SECTION("isA<Null> matches a null node.", "[JSON][Node][Reference]")
  {
    Node jNode(nullptr);
    REQUIRE(isA<Null>(jNode));
    REQUIRE_FALSE(isA<Boolean>(jNode));
  }
  SECTION("isA<Array> matches an array node.", "[JSON][Node][Reference]")
  {
    Node jNode{ 1, 2, 3 };
    REQUIRE(isA<Array>(jNode));
    REQUIRE_FALSE(isA<Object>(jNode));
  }
  SECTION("isA<Object> matches an object node.", "[JSON][Node][Reference]")
  {
    Node jNode{ { "k", 1 } };
    REQUIRE(isA<Object>(jNode));
    REQUIRE_FALSE(isA<Array>(jNode));
  }
  SECTION("isA<Hole> matches a hole node.", "[JSON][Node][Reference]")
  {
    Node jNode = Node::make<Hole>();
    REQUIRE(isA<Hole>(jNode));
    REQUIRE_FALSE(isA<Number>(jNode));
  }
}

TEST_CASE("Check additional cross-type NRef exceptions.", "[JSON][Node][Reference][Exception]")
{
  SECTION("Number reference an array.", "[JSON][Node][Reference][Number][Exception]")
  {
    Node jNode{ 1, 2, 3 };
    REQUIRE_THROWS_AS(NRef<Number>(jNode).value<int>(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Number>(jNode).value<int>(), "Node Error: Node not a number.");
  }
  SECTION("String reference an object.", "[JSON][Node][Reference][String][Exception]")
  {
    Node jNode{ { "k", 1 } };
    REQUIRE_THROWS_AS(NRef<String>(jNode).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<String>(jNode).value(), "Node Error: Node not a string.");
  }
  SECTION("Boolean reference a number.", "[JSON][Node][Reference][Boolean][Exception]")
  {
    Node jNode(42);
    REQUIRE_THROWS_AS(NRef<Boolean>(jNode).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Boolean>(jNode).value(), "Node Error: Node not a boolean.");
  }
  SECTION("Null reference a string.", "[JSON][Node][Reference][Null][Exception]")
  {
    Node jNode("hello");
    REQUIRE_THROWS_AS(NRef<Null>(jNode).value(), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Null>(jNode).value(), "Node Error: Node not a null.");
  }
  SECTION("Hole reference a string throws.", "[JSON][Node][Reference][Exception]")
  {
    Node jNode("hello");
    REQUIRE_THROWS_AS(NRef<Hole>(jNode), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Hole>(jNode), "Node Error: Node not a hole.");
  }
}