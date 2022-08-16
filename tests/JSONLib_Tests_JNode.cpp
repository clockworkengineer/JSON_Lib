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
// ==================
// JNode constructors
// ==================
TEST_CASE("Use of JNode constructors", "[JSON][JNode][Constructor]") {
  SECTION("Test JNode(integer).", "[JSON][JNode][Constructor][Integer]") {
    JNode jNode(999);
    REQUIRE(jNode.getType() == JNodeType::number);
    REQUIRE_FALSE(!JRef<Number>(jNode).number().isInt());
    REQUIRE(JRef<Number>(jNode).number().getInt() == 999);
  }
  SECTION("Test JNode(long).", "[JSON][JNode][Constructor][Long]") {
    JNode jNode(99988899l);
    REQUIRE(jNode.getType() == JNodeType::number);
    REQUIRE_FALSE(!JRef<Number>(jNode).number().isLong());
    REQUIRE(JRef<Number>(jNode).number().getInt() == 99988899l);
  }
  SECTION("Test JNode(float).", "[JSON][JNode][Constructor][Float]") {
    JNode jNode(777.999f);
    REQUIRE(jNode.getType() == JNodeType::number);
    REQUIRE_FALSE(!JRef<Number>(jNode).number().isFloat());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).number().getFloat(),
                                      777.999f, 0.0001));
  }
  SECTION("Test JNode(double).", "[JSON][JNode][Constructor][Double]") {
    JNode jNode(66666.8888);
    REQUIRE(jNode.getType() == JNodeType::number);
    REQUIRE_FALSE(!JRef<Number>(jNode).number().isDouble());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).number().getDouble(),
                                      66666.8888, 0.0001));
  }
  SECTION("Test JNode(C string).", "[JSON][JNode][Constructor][CString]") {
    JNode jNode("test string");
    REQUIRE(jNode.getType() == JNodeType::string);
    REQUIRE(JRef<String>(jNode).string() == "test string");
  }
  SECTION("Test JNode(string).", "[JSON][JNode][Constructor]") {
    JNode jNode("test string");
    REQUIRE(jNode.getType() == JNodeType::string);
    REQUIRE(JRef<String>(jNode).string() == "test string");
  }
  SECTION("Test JNode(boolean).", "[JSON][JNode][Constructor][String]") {
    JNode jNode(false);
    REQUIRE(jNode.getType() == JNodeType::boolean);
    REQUIRE_FALSE(JRef<Boolean>(jNode).boolean());
  }
  SECTION("Test JNode(null).", "[JSON][JNode][Constructor][Null]") {
    JNode jNode(nullptr);
    REQUIRE(jNode.getType() == JNodeType::null);
    REQUIRE(JRef<Null>(jNode).null() == nullptr);
  }
  SECTION("Test JNode(array).", "[JSON][JNode][Constructor][Array]") {
    JNode jNode{1, 2, 3, 4};
    REQUIRE(jNode.getType() == JNodeType::array);
    auto &array = JRef<Array>(jNode).array();
    REQUIRE(JRef<Number>(array[0]).number().getInt() == 1);
    REQUIRE(JRef<Number>(array[1]).number().getInt() == 2);
    REQUIRE(JRef<Number>(array[2]).number().getInt() == 3);
    REQUIRE(JRef<Number>(array[3]).number().getInt() == 4);
  }
  SECTION("Test JNode(object).", "[JSON][JNode][Constructor][Object]") {
    JNode jNode{{"key1", 55}, {"key2", 26666}};
    REQUIRE(jNode.getType() == JNodeType::object);
    auto &object = JRef<Object>(jNode);
    REQUIRE(JRef<Number>(object["key1"]).number().getInt() == 55);
    REQUIRE(JRef<Number>(object["key2"]).number().getInt() == 26666);
  }
}
// ==============
// JNode Indexing
// ==============
TEST_CASE("Use of JNode indexing operators", "[JSON][JNode][Index]") {
  const JSON json;
  SECTION("Parse dictionary and check its components using indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{R"({"City":"Southampton","Population":500000})"};
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JRef<String>((json.root())["City"]).string() == "Southampton");
    REQUIRE(JRef<Number>((json.root())["Population"]).toString() == "500000");
  }
  SECTION("Parse list and check its components using indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{R"([777,9000,"apples"])"};
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).toString() == "777");
    REQUIRE(JRef<Number>((json.root())[1]).toString() == "9000");
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse list with embedded dictionary and check its components using "
          "indexing",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{
        R"([777,{"City":"Southampton","Population":500000},"apples"])"};
    json.parse(jsonSource);
    REQUIRE(JRef<Number>((json.root())[0]).toString() == "777");
    REQUIRE(JRef<String>((json.root())[1]["City"]).string() == "Southampton");
    REQUIRE(JRef<Number>((json.root())[1]["Population"]).toString() ==
            "500000");
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{R"({"City":"Southampton","Population":500000})"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].getType() == JNodeType::object,
                      JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].getType() == JNodeType::object,
                        "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception",
          "[JSON][JNode][Index]") {
    BufferSource jsonSource{R"([777,9000,"apples"])"};
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
    BufferSource jsonSource{R"("0123456789")"};
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).string() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{R"([777,9000,"apples"])"};
    json.parse(jsonSource);
    REQUIRE(JRef<Array>(json.root()).size() == 3);
    REQUIRE(JRef<String>((json.root())[2]).string() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]") {
    BufferSource jsonSource{R"({"City":"Southampton","Population":500000 })"};
    json.parse(jsonSource);
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).string() == "Southampton");
  }
}