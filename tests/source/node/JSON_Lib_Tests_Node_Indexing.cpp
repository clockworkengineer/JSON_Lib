#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check use of Node indexing operators.", "[JSON][Node][Index]")
{
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"({"City":"Southampton","Population":500000})" };
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(NRef<String>((json.root())["City"]).value() == "Southampton");
    REQUIRE(NRef<Number>((json.root())["Population"]).value<int>() == 500000);
  }
  SECTION("Parse array and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"([777,9000,"apples"])" };
    json.parse(jsonSource);
    REQUIRE(NRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(NRef<Number>((json.root())[1]).value<int>() == 9000);
    REQUIRE(NRef<String>((json.root())[2]).value() == "apples");
  }
  SECTION("Parse array with embedded dictionary and check its components using indexing.", "[JSON][Node][Index]")
  {
    BufferSource jsonSource{ R"([777,{"City":"Southampton","Population":500000},"apples"])" };
    json.parse(jsonSource);
    REQUIRE(NRef<Number>((json.root())[0]).value<int>() == 777);
    REQUIRE(NRef<String>((json.root())[1]["City"]).value() == "Southampton");
    REQUIRE(NRef<Number>((json.root())[1]["Population"]).value<int>() == 500000);
    REQUIRE(NRef<String>((json.root())[2]).value() == "apples");
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
  SECTION("Parse nested object and index into inner object.", "[JSON][Node][Index]")
  {
    json.parse(BufferSource{ R"({"outer":{"inner":42}})" });
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(isA<Object>(json.root()["outer"]));
    REQUIRE(NRef<Number>(json.root()["outer"]["inner"]).value<int>() == 42);
  }
  SECTION("Parse nested array and index into inner array.", "[JSON][Node][Index]")
  {
    json.parse(BufferSource{ R"([[1,2],[3,4]])" });
    REQUIRE(isA<Array>(json.root()));
    REQUIRE(NRef<Number>(json.root()[0][0]).value<int>() == 1);
    REQUIRE(NRef<Number>(json.root()[0][1]).value<int>() == 2);
    REQUIRE(NRef<Number>(json.root()[1][0]).value<int>() == 3);
    REQUIRE(NRef<Number>(json.root()[1][1]).value<int>() == 4);
  }
  SECTION("Modify object value via non-const key index.", "[JSON][Node][Index]")
  {
    JSON jsonMut;
    jsonMut.parse(BufferSource{ R"({"City":"Southampton","Population":500000})" });
    jsonMut.root()["City"] = std::string("London");
    REQUIRE(NRef<String>(jsonMut.root()["City"]).value() == "London");
    REQUIRE(NRef<Number>(jsonMut.root()["Population"]).value<int>() == 500000);
  }
  SECTION("Modify array element via non-const numeric index.", "[JSON][Node][Index]")
  {
    JSON jsonMut;
    jsonMut.parse(BufferSource{ R"([1,2,3])" });
    jsonMut.root()[1] = 99;
    REQUIRE(NRef<Number>(jsonMut.root()[0]).value<int>() == 1);
    REQUIRE(NRef<Number>(jsonMut.root()[1]).value<int>() == 99);
    REQUIRE(NRef<Number>(jsonMut.root()[2]).value<int>() == 3);
  }
  SECTION("Const reference indexing into parsed object.", "[JSON][Node][Index]")
  {
    json.parse(BufferSource{ R"({"key":123})" });
    const Node &constRoot = json.root();
    REQUIRE(NRef<Number>(constRoot["key"]).value<int>() == 123);
  }
  SECTION("Const reference indexing into parsed array.", "[JSON][Node][Index]")
  {
    json.parse(BufferSource{ R"([10,20,30])" });
    const Node &constRoot = json.root();
    REQUIRE(NRef<Number>(constRoot[0]).value<int>() == 10);
    REQUIRE(NRef<Number>(constRoot[1]).value<int>() == 20);
    REQUIRE(NRef<Number>(constRoot[2]).value<int>() == 30);
  }
  SECTION("Index into deeply nested structure.", "[JSON][Node][Index]")
  {
    json.parse(BufferSource{ R"({"a":{"b":{"c":99}}})" });
    REQUIRE(NRef<Number>(json.root()["a"]["b"]["c"]).value<int>() == 99);
  }
}

TEST_CASE("Check Node indexing on directly constructed nodes.", "[JSON][Node][Index]")
{
  SECTION("Build object via key indexing on a Hole Node.", "[JSON][Node][Index]")
  {
    Node jNode = Node::make<Hole>();
    jNode["name"] = std::string("Alice");
    REQUIRE(isA<Object>(jNode));
    REQUIRE(NRef<String>(jNode["name"]).value() == "Alice");
  }
  SECTION("Build array via numeric indexing on a Hole Node (auto-resize).", "[JSON][Node][Index]")
  {
    Node jNode = Node::make<Hole>();
    jNode[2] = 42;
    REQUIRE(isA<Array>(jNode));
    REQUIRE(NRef<Array>(jNode).size() == 3);
    REQUIRE(isA<Hole>(jNode[0]));
    REQUIRE(isA<Hole>(jNode[1]));
    REQUIRE(NRef<Number>(jNode[2]).value<int>() == 42);
  }
  SECTION("Array built from initializer, elements accessible by index.", "[JSON][Node][Index]")
  {
    Node jNode{ 10, 20, 30 };
    REQUIRE(NRef<Number>(jNode[0]).value<int>() == 10);
    REQUIRE(NRef<Number>(jNode[1]).value<int>() == 20);
    REQUIRE(NRef<Number>(jNode[2]).value<int>() == 30);
  }
  SECTION("Object built from initializer, entries accessible by key.", "[JSON][Node][Index]")
  {
    Node jNode{ { "x", 1 }, { "y", 2 } };
    REQUIRE(NRef<Number>(jNode["x"]).value<int>() == 1);
    REQUIRE(NRef<Number>(jNode["y"]).value<int>() == 2);
  }
}