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
TEST_CASE("Use of JNode indexing operators", "[JSON][JNode][Index]") {
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JRef<String>((json.root())["City"]).string() ==
            "Southampton");
    REQUIRE(JRef<Number>((json.root())["Population"]).toString() ==
            "500000");
  }
  SECTION("Parse list and check its components using indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).toString() == "777");
    REQUIRE(JRef<Number>((json.root())[1]).toString() == "9000");
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse list with embedded dictionary and check its components using "
          "indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{
        "[777,{\"City\":\"Southampton\",\"Population\":500000},\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).toString() == "777");
    REQUIRE(JRef<String>((json.root())[1]["City"]).string() ==
            "Southampton");
    REQUIRE(JRef<Number>((json.root())[1]["Population"]).toString() ==
            "500000");
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].getType() == JNodeType::object,
                      JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].getType() ==
                            JNodeType::object,
                        "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())[3].getType() == JNodeType::array,
                      JNode::Error);
    REQUIRE_THROWS_WITH((json.root())[3].getType() == JNodeType::array,
                        "JNode Error: Invalid index used to access array.");
  }
}
// =========================
// JNode Reference functions
// =========================
TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]") {
  const JSON json;
  SECTION("Integer reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{"45500"};
    json.parse(jsonSource);
    REQUIRE(JRef<Number>(json.root()).toString() == "45500");
  }
  SECTION("String reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{"\"0123456789\""};
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).string() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{
        "{\"City\":\"Southampton\",\"Population\":500000 }"};
    json.parse(jsonSource);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).string() ==
            "Southampton");
  }
}