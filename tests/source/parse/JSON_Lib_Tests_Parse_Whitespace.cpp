#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON parsing of various whitespace characters are ignored.", "[JSON][Parse][Whitespace]")
{
  const JSON json;
  std::string ws = "";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with no whitespace.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} with no whitespace.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += " ";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' '.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(
    R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' '.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\t";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(
    R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' \t'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\n";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t\n'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(
    R"(Parse object {"City":"Southampton","Population":500000} whitespace. ' \t\n'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\r";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t\n\r'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' \t\n\r'.)",
    "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
}
TEST_CASE("Check whitespace is ignored around simple scalar values.", "[JSON][Parse][Whitespace][Scalar]")
{
  const JSON json;
  SECTION("Whitespace surrounding a number is ignored.", "[JSON][Parse][Whitespace][Scalar]")
  {
    json.parse(BufferSource{ "  \t 42 \n " });
    REQUIRE(isA<Number>(json.root()));
    REQUIRE(NRef<Number>(json.root()).value<int>() == 42);
  }
  SECTION("Whitespace surrounding true is ignored.", "[JSON][Parse][Whitespace][Scalar]")
  {
    json.parse(BufferSource{ " \t true \r\n" });
    REQUIRE(isA<Boolean>(json.root()));
    REQUIRE(NRef<Boolean>(json.root()).value() == true);
  }
  SECTION("Whitespace surrounding false is ignored.", "[JSON][Parse][Whitespace][Scalar]")
  {
    json.parse(BufferSource{ "\n false\t" });
    REQUIRE(isA<Boolean>(json.root()));
    REQUIRE(NRef<Boolean>(json.root()).value() == false);
  }
  SECTION("Whitespace surrounding null is ignored.", "[JSON][Parse][Whitespace][Scalar]")
  {
    json.parse(BufferSource{ "  null  " });
    REQUIRE(isA<Null>(json.root()));
  }
  SECTION("Whitespace surrounding a string is ignored.", "[JSON][Parse][Whitespace][Scalar]")
  {
    json.parse(BufferSource{ "  \"hello\"  " });
    REQUIRE(isA<String>(json.root()));
    REQUIRE(NRef<String>(json.root()).value() == "hello");
  }
}
TEST_CASE("Check values are correct after parsing with whitespace.", "[JSON][Parse][Whitespace][Values]")
{
  const JSON json;
  SECTION(R"(Array ["Dog",1964,true,null] values correct after parsing with ' \t\n' whitespace.)",
    "[JSON][Parse][Whitespace][Values]")
  {
    std::string ws = " \t\n";
    json.parse(BufferSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" });
    const auto &arr = NRef<Array>(json.root());
    REQUIRE(NRef<String>(arr[0]).value() == "Dog");
    REQUIRE(NRef<Number>(arr[1]).value<int>() == 1964);
    REQUIRE(NRef<Boolean>(arr[2]).value() == true);
    REQUIRE(isA<Null>(arr[3]));
  }
  SECTION(R"(Object {"City":"Southampton","Population":500000} values correct after parsing with ' \t\n' whitespace.)",
    "[JSON][Parse][Whitespace][Values]")
  {
    std::string ws = " \t\n";
    json.parse(BufferSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" });
    const auto &obj = NRef<Object>(json.root());
    REQUIRE(NRef<String>(obj["City"]).value() == "Southampton");
    REQUIRE(NRef<Number>(obj["Population"]).value<int>() == 500000);
  }
}
TEST_CASE("Check JSON parsing of Windows-style (\\r\\n) line endings.", "[JSON][Parse][Whitespace][CRLF]")
{
  const JSON json;
  SECTION(R"(Parse array with \r\n line endings.)", "[JSON][Parse][Whitespace][CRLF]")
  {
    json.parse(BufferSource{ "[\r\n  \"item1\",\r\n  42\r\n]" });
    REQUIRE(isA<Array>(json.root()));
    const auto &arr = NRef<Array>(json.root());
    REQUIRE(NRef<String>(arr[0]).value() == "item1");
    REQUIRE(NRef<Number>(arr[1]).value<int>() == 42);
  }
  SECTION(R"(Parse object with \r\n line endings.)", "[JSON][Parse][Whitespace][CRLF]")
  {
    json.parse(BufferSource{ "{\r\n  \"key\": \"value\"\r\n}" });
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(NRef<String>(NRef<Object>(json.root())["key"]).value() == "value");
  }
}
TEST_CASE("Check JSON parsing of pretty-printed (multiline) JSON.", "[JSON][Parse][Whitespace][Multiline]")
{
  const JSON json;
  SECTION("Parse pretty-printed object with newlines and indentation.", "[JSON][Parse][Whitespace][Multiline]")
  {
    const std::string prettyJSON = R"({
  "name": "Alice",
  "age": 30,
  "active": true
})";
    REQUIRE_NOTHROW(json.parse(BufferSource{ prettyJSON }));
    REQUIRE(isA<Object>(json.root()));
    const auto &obj = NRef<Object>(json.root());
    REQUIRE(NRef<String>(obj["name"]).value() == "Alice");
    REQUIRE(NRef<Number>(obj["age"]).value<int>() == 30);
    REQUIRE(NRef<Boolean>(obj["active"]).value() == true);
  }
  SECTION("Parse pretty-printed nested array with newlines.", "[JSON][Parse][Whitespace][Multiline]")
  {
    const std::string prettyJSON = R"([
  1,
  2,
  3
])";
    REQUIRE_NOTHROW(json.parse(BufferSource{ prettyJSON }));
    REQUIRE(isA<Array>(json.root()));
    const auto &arr = NRef<Array>(json.root());
    REQUIRE(NRef<Number>(arr[0]).value<int>() == 1);
    REQUIRE(NRef<Number>(arr[2]).value<int>() == 3);
  }
}