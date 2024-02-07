//
// Unit Tests: JSON_Lib_Tests_Parse_Escapes
//
// Description: JSON parse escaped characters unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// =================================
// Parse JSON With Escape Characters
// =================================
TEST_CASE("Check JSON object for decoding of strings with escape characters.", "[JSON][Parse][Escapes]")
{
  const JSON json;
  SECTION("Parse JSON string with escapes '\\t' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \t ");
  }
  SECTION("Parse JSON string with escapes '\\\"' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \" ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == R"(abcdefghijklmnopqrstuvwxyz " )");
  }
  SECTION("Parse JSON string with escapes '\\\\' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \\ ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == R"(abcdefghijklmnopqrstuvwxyz \ )");
  }
  SECTION("Parse JSON string with escapes '\\b' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \b ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \b ");
  }
  SECTION("Parse JSON string with escapes '\\/' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \/ ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz / ");
  }
  SECTION("Parse JSON string with escapes '\\f' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \f ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \f ");
  }
  SECTION("Parse JSON string with escapes '\\n' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \n ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \n ");
  }
  SECTION("Parse JSON string with escapes '\\r' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \r ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \r ");
  }
  SECTION("Parse JSON string with escapes '\\t' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    json.parse(jsonSource);
    REQUIRE(JRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \t ");
  }
  SECTION("Parse JSON string with escapes '\\u0123' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0123 ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0123 " };
    REQUIRE(JRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes '\\u0123 \\u0456' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0123 \u0456 ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0123 \u0456 " };
    REQUIRE(JRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes  '\\uD834\\uDD1E' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz  \uD834\uDD1E ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz  \U0001D11E " };
    REQUIRE(JRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes '\\u0000 \\0001 \\0002 \\0003' to buffer and check value.",
    "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0005 \u0004 \u0003 \u0002 \u0001")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0005 \u0004 \u0003 \u0002 \u0001" };
    auto test = JRef<String>(json.root()).value();
    REQUIRE(JRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
}