//
// Unit Tests: JSON
//
// Description: JNode access/index/reference unit tests for JSON class 
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
// ==============
// JNode Indexing
// ==============
TEST_CASE("Use of JNode indexing operators", "[JSON][JNode][Index]")
{
  JSON json;
  SECTION("Parse dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JNodeRef<JNodeString>((json.root())["City"]).string() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())["Population"]).toString() == "500000");
  }
  SECTION("Parse list and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[0]).toString() == "777");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[1]).toString() == "9000");
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse list with embedded dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,{\"City\":\"Southampton\",\"Population\":500000},\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[0]).toString() == "777");
    REQUIRE(JNodeRef<JNodeString>((json.root())[1]["City"]).string() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[1]["Population"]).toString() == "500000");
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].getNodeType() == JNodeType::object, JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].getNodeType() == JNodeType::object, "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())[3].getNodeType() == JNodeType::array, JNode::Error);
    REQUIRE_THROWS_WITH((json.root())[3].getNodeType() == JNodeType::array, "JNode Error: Invalid index used to access array.");
  }
}
// =========================
// JNode Reference functions
// =========================
TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]")
{
  JSON json;
  SECTION("Integer reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"45500"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).toString() == "45500");
  }
  SECTION("String reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"\"0123456789\""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeString>(json.root()).string() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeArray>(json.root()).size() == 3);
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000 }"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeObject>(json.root()).size() == 2);
    REQUIRE(JNodeRef<JNodeString>((json.root())["City"]).string() == "Southampton");
  }
}
// =======================
// JNode Number Conversion
// =======================
TEST_CASE("Check JNodeNumber number conversion", "[JSON][JNode][JNodeNumber]")
{
  JSON json;
  SECTION("Floating point not converted to Integer", "[JSON][JNode][JNodeNumber]")
  {
    long long longValue;
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(json.root()).integer(longValue));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]")
  {
    long double doubleValue;
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).floatingpoint(doubleValue));
    REQUIRE(doubleValue == 678.8990);
  }
  SECTION("Integer point converted to Long", "[JSON][JNode][JNodeNumber]")
  {
    long long longValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).integer(longValue));
    REQUIRE(longValue == 78989);
  }
  SECTION("Integer point not converted to double", "[JSON][JNode][JNodeNumber]")
  {
    long double doubleValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).floatingpoint(doubleValue));
  }
  SECTION("Check  floating point with exponent", "[JSON][JNode][JNodeNumber][Exception")
  {
    BufferSource jsonSource{"78.43e-2"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.e43e-2"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Syntax error detected.");
  }
  SECTION("Check floating point with multiple decimal points", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.5454.545"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Syntax error detected.");
  }
}