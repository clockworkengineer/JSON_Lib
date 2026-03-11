#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON create complex JSON structures.", "[JSON][Create][Complex]")
{
  SECTION("A Single level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null})");
  }
  SECTION("A two level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString()
            == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42}})");
  }
  SECTION("A three level object.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"][5] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"][5] = 42;
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":[null,null,null,null,null,"Niels"],"nothing":null,"answer":{"everything":[null,null,null,null,null,42]}})");
  }
  SECTION("Object with sub array/object create using initializer list.", "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
  SECTION(
    "Object with sub array/object with an embedded array create using "
    "initializer list.",
    "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", Node{ 1, 2, 3, 4, 5 } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":[1,2,3,4,5]}})");
  }
  SECTION(
    "Object with sub array/object with an embedded object create using "
    "initializer list.",
    "[JSON][Create][Complex]")
  {
    JSON json;
    json["pi"] = 3.141;
    json["happy"] = true;
    json["name"] = "Niels";
    json["nothing"] = nullptr;
    json["answer"]["everything"] = 42;
    json["list"] = { 1, 0, 2 };
    json["object"] = { { "currency", "USD" }, { "value", Node{ { "key1", 22 }, { "key2", 99.899 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":{"key1":22,"key2":99.899}}})");
  }
  SECTION("Array creation completely using a initializer list.", "[JSON][Create][Complex][Initializer]")
  {
    JSON json = { 1, 2, 3, 4 };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([1,2,3,4])");
  }
  SECTION("Object creation completely using a initializer list.", "[JSON][Create][Complex][Initializer]")
  {
    JSON json = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"({"currency":"USD","value":42.99})");
  }
  SECTION("Object creation completely using a nested initializer list.", "[JSON][Create][Complex][Initializer]")
  {
    // Note: For the moment has to explicitly uses Node to create a
    // nested object/array
    JSON json = { { "pi", 3.141 },
      { "happy", true },
      { "name", "Niels" },
      { "nothing", nullptr },
      { "answer", Node{ { "everything", 42 } } },
      { "list", Node{ 1, 0, 2 } },
      { "object", Node{ { "currency", "USD" }, { "value", 42.99 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
  SECTION("Build complex object, stringify, re-parse and validate tree.", "[JSON][Create][Complex][RoundTrip]")
  {
    JSON json;
    json["city"] = "London";
    json["pop"] = 9000000;
    json["active"] = true;
    BufferDestination dest;
    json.stringify(dest);
    const JSON reparsed(dest.toString());
    REQUIRE_FALSE(!isA<Object>(reparsed.root()));
    REQUIRE_FALSE(!isA<String>(reparsed.root()["city"]));
    REQUIRE_FALSE(!isA<Number>(reparsed.root()["pop"]));
    REQUIRE_FALSE(!isA<Boolean>(reparsed.root()["active"]));
    REQUIRE(NRef<String>(reparsed.root()["city"]).value() == "London");
    REQUIRE(NRef<Number>(reparsed.root()["pop"]).value<int>() == 9000000);
    REQUIRE(NRef<Boolean>(reparsed.root()["active"]).value() == true);
  }
  SECTION("Create an array of objects and stringify.", "[JSON][Create][Complex][ArrayOfObjects]")
  {
    JSON json;
    json[0] = Node{ { { "name", "Alice" }, { "age", 30 } } };
    json[1] = Node{ { { "name", "Bob" }, { "age", 25 } } };
    BufferDestination dest;
    REQUIRE_NOTHROW(json.stringify(dest));
    REQUIRE(dest.toString() == R"([{"name":"Alice","age":30},{"name":"Bob","age":25}])");
  }
  SECTION("Overwrite a key in a complex object and re-stringify.", "[JSON][Create][Complex][Overwrite]")
  {
    JSON json;
    json["x"] = 1;
    json["y"] = 2;
    json["x"] = 99;
    BufferDestination dest;
    json.stringify(dest);
    REQUIRE(dest.toString() == R"({"x":99,"y":2})");
  }
  SECTION("Deep mixed nesting: array inside array inside object.", "[JSON][Create][Complex][Deep]")
  {
    JSON json;
    json["grid"][0][0] = 1;
    json["grid"][0][1] = 2;
    json["grid"][1][0] = 3;
    json["grid"][1][1] = 4;
    BufferDestination dest;
    REQUIRE_NOTHROW(json.stringify(dest));
    REQUIRE(dest.toString() == R"({"grid":[[1,2],[3,4]]})");
  }
  SECTION("print() on a complex structure produces non-empty formatted output.", "[JSON][Create][Complex][Print]")
  {
    JSON json;
    json["name"] = "test";
    json["values"] = Node{ 1, 2, 3 };
    BufferDestination dest;
    REQUIRE_NOTHROW(json.print(dest));
    REQUIRE_FALSE(dest.toString().empty());
    REQUIRE(dest.toString().find("test") != std::string::npos);
  }
  SECTION("strip() removes whitespace from a complex JSON string.", "[JSON][Create][Complex][Strip]")
  {
    const std::string pretty = R"({  "key" : [ 1 , 2 , 3 ]  })";
    BufferDestination dest;
    JSON::strip(BufferSource{ pretty }, dest);
    REQUIRE(dest.toString() == R"({"key":[1,2,3]})");
  }
}