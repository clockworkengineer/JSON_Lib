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
    json["object"] = { { "currency", "USD" }, { "value", JNode{ 1, 2, 3, 4, 5 } } };
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
    json["object"] = { { "currency", "USD" }, { "value", JNode{ { "key1", 22 }, { "key2", 99.899 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":{"key1":22,"key2":99.899}}})");
  }
  SECTION("Array creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { 1, 2, 3, 4 };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"([1,2,3,4])");
  }
  SECTION("Object creation completely using a initializer list.", "[JSON][Create][Complex][Initializer")
  {
    JSON json = { { "currency", "USD" }, { "value", 42.99 } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(jsonDestination.toString() == R"({"currency":"USD","value":42.99})");
  }
  SECTION("Object creation completely using a nested initializer list.", "[JSON][Create][Complex][Initializer")
  {
    // Note: For the moment has to explicitly uses JNode to create a
    // nested object/array
    JSON json = { { "pi", 3.141 },
      { "happy", true },
      { "name", "Niels" },
      { "nothing", nullptr },
      { "answer", JNode{ { "everything", 42 } } },
      { "list", JNode{ 1, 0, 2 } },
      { "object", JNode{ { "currency", "USD" }, { "value", 42.99 } } } };
    BufferDestination jsonDestination;
    REQUIRE_NOTHROW(json.stringify(jsonDestination));
    REQUIRE(
      jsonDestination.toString()
      == R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"object":{"currency":"USD","value":42.99}})");
  }
}