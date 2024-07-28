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
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a integer.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 300;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(JRef<Number>(json.root()["integer"]).value<int>() == 300);
  }
  SECTION("Initialise root JSON JNode with one entry containing a long.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["integer"] = 30000l;
    REQUIRE_FALSE(!isA<Number>(json["integer"]));
    REQUIRE(JRef<Number>(json.root()["integer"]).value<long>() == 30000);
  }
  SECTION("Initialise root JSON JNode with one entry containing a float.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141f;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<float>(), 3.141f, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a double.", "[JSON][Create][Object][Number]")
  {
    JSON json;
    json["pi"] = 3.141;
    REQUIRE_FALSE(!isA<Number>(json["pi"]));
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(json.root()["pi"]).value<double>(), 3.141, 0.0001));
  }
  SECTION("Initialise root JSON JNode with one entry containing a const char *.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = "robert";
    REQUIRE_FALSE(!isA<String>(json["name"]));
    REQUIRE(JRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a std::string.", "[JSON][Create][Object][String]")
  {
    JSON json;
    json["name"] = std::string{ "robert" };
    REQUIRE_FALSE(!isA<String>(json["name"]));
    REQUIRE(JRef<String>(json.root()["name"]).value() == "robert");
  }
  SECTION("Initialise root JSON JNode with one entry containing a boolean.", "[JSON][Create][Object][Boolean]")
  {
    JSON json;
    json["flag"] = true;
    REQUIRE_FALSE(!isA<Boolean>(json["flag"]));
    REQUIRE_FALSE(!JRef<Boolean>(json.root()["flag"]).value());
  }
  SECTION("Initialise root JSON JNode with one entry containing a null.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]));
    REQUIRE(JRef<Null>(json.root()["nothing"]).value() == nullptr);
  }
  SECTION("Create two level object and null at base.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]["extra"]));
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]).value() == nullptr);
  }
  SECTION("Create three level object and null at base and stringify.", "[JSON][Create][Object][null]")
  {
    JSON json;
    json["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!isA<Null>(json["nothing"]["extra"]["more"]));
    REQUIRE(JRef<Null>(json.root()["nothing"]["extra"]["more"]).value() == nullptr);
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(json.stringify(destinationBuffer));
    REQUIRE(destinationBuffer.toString() == R"({"nothing":{"extra":{"more":null}}})");
  }
}