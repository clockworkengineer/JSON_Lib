#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON object creation api.", "[JSON][Create][Object]")
{
  SECTION("Initialise JSON with JSON object passed to constructor.", "[JSON][Create][Constructor]")
  {
    REQUIRE_NOTHROW(JSON(R"({ "pi" : 3.141 })"));
  }
  SECTION(
    "Initialise JSON with JSON object passed to constructor and validate.", "[JSON][Create][Constructor][Validate]")
  {
    const JSON json(R"({ "pi" : 3.141 })");
    REQUIRE_FALSE(!isA<Object>(json.root()));
    REQUIRE_FALSE(!isA<Number>(json.root()["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON Node with one entry containing a integer.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 300;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(NRef<Number>(json.root()["integer"]).value<int>() == 300);
  }
  SECTION("Initialise root JSON Node with one entry containing a long.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 30000l;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(NRef<Number>(json.root()["integer"]).value<long>() == 30000);
  }
  SECTION("Initialise root JSON Node with one entry containing a float.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141f;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON Node with one entry containing a double.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()["pi"]).value<double>(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON Node with one entry containing a const char *.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = "robert";
    REQUIRE_FALSE(!isA<String>(json["name"]));
    REQUIRE(NRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON Node with one entry containing a std::string.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = std::string{ "robert" };
    REQUIRE_FALSE(!isA<String>(json["name"]));
    REQUIRE(NRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON Node with one entry containing a boolean.", "[JSON][Create][Object][Boolean]")
  {
    JSON json;
    json["flag"] = true;
    REQUIRE_FALSE(!isA<Boolean>(json["flag"]));
    REQUIRE_FALSE(!NRef<Boolean>(json.root()["flag"]).value());
  }
  SECTION("Initialise root JSON Node with one entry containing a null.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]));
    REQUIRE(NRef<Null>(json.root()["nothing"]).value() == nullptr);
  }
  SECTION("Create two level object and null at base.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]["extra"]));
    REQUIRE(NRef<Null>(json.root()["nothing"]["extra"]).value() == nullptr);
  }
  SECTION("Create three level object and null at base and stringify.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]["extra"]["more"]));
    REQUIRE(NRef<Null>(json.root()["nothing"]["extra"]["more"]).value() == nullptr);
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(json.stringify(destinationBuffer));
    REQUIRE(destinationBuffer.toString() == R"({"nothing":{"extra":{"more":null}}})");
  }
  SECTION("Initialise root JSON Node with one entry containing a long long.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["big"] = 9000000000ll;
    REQUIRE_FALSE(!isA<Number>(json["big"]));
    REQUIRE(NRef<Number>(json.root()["big"]).value<long long>() == 9000000000ll);
  }
  SECTION("Initialise root JSON Node with one entry containing a long double.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141l;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()["pi"]).value<long double>(), 3.141l, 0.0001));
  }
  SECTION("Initialise root JSON Node with one entry containing false boolean.", "[JSON][Create][Object][Boolean]")
  {
    JSON json;
    json["flag"] = false;
    REQUIRE_FALSE(!isA<Boolean>(json["flag"]));
    REQUIRE(NRef<Boolean>(json.root()["flag"]).value() == false);
  }
  SECTION("Create object and verify contains() for present and absent keys.", "[JSON][Create][Object][Contains]")
  {
    JSON json;
    json["name"] = "Alice";
    json["age"] = 30;
    REQUIRE(NRef<Object>(json.root()).contains("name"));
    REQUIRE(NRef<Object>(json.root()).contains("age"));
    REQUIRE_FALSE(NRef<Object>(json.root()).contains("missing"));
  }
  SECTION("Create object and verify size().", "[JSON][Create][Object][Size]")
  {
    JSON json;
    json["a"] = 1;
    json["b"] = 2;
    json["c"] = 3;
    REQUIRE(NRef<Object>(json.root()).size() == 3);
  }
  SECTION("Access non-existent key on const object throws Node::Error.", "[JSON][Create][Object][Exception]")
  {
    const JSON json(R"({"x":1})");
    REQUIRE_THROWS_AS(json.root()["missing"], Node::Error);
    REQUIRE_THROWS_WITH(json.root()["missing"], "Node Error: Invalid key used to access object.");
  }
  SECTION("Create JSON object using ObjectInitializer constructor and validate.", "[JSON][Create][Object][Initializer]")
  {
    const JSON json{ JSON::ObjectInitializer{ { "city", "London" }, { "pop", 9000000 }, { "active", true } } };
    REQUIRE_FALSE(!isA<Object>(json.root()));
    REQUIRE(NRef<Object>(json.root()).size() == 3);
    REQUIRE(NRef<String>(json.root()["city"]).value() == "London");
    REQUIRE(NRef<Number>(json.root()["pop"]).value<int>() == 9000000);
    REQUIRE(NRef<Boolean>(json.root()["active"]).value() == true);
  }
  SECTION("Build object, stringify, re-parse and validate.", "[JSON][Create][Object][RoundTrip]")
  {
    JSON json;
    json["x"] = 42;
    json["y"] = "hello";
    BufferDestination dest;
    json.stringify(dest);
    const JSON reparsed(dest.toString());
    REQUIRE_FALSE(!isA<Object>(reparsed.root()));
    REQUIRE(NRef<Number>(reparsed.root()["x"]).value<int>() == 42);
    REQUIRE(NRef<String>(reparsed.root()["y"]).value() == "hello");
  }
  SECTION("print() on an object produces non-empty formatted output.", "[JSON][Create][Object][Print]")
  {
    const JSON json(R"({"name":"test","value":99})");
    BufferDestination dest;
    REQUIRE_NOTHROW(json.print(dest));
    REQUIRE_FALSE(dest.toString().empty());
    REQUIRE(dest.toString().find("test") != std::string::npos);
    REQUIRE(dest.toString().find("99") != std::string::npos);
  }
}