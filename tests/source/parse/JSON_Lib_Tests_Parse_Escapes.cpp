#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON object for decoding of strings with escape characters.", "[JSON][Parse][Escapes]")
{
  const JSON json;
  SECTION("Parse JSON string with escapes '\\t' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \t ");
  }
  SECTION("Parse JSON string with escapes '\\\"' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \" ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == R"(abcdefghijklmnopqrstuvwxyz " )");
  }
  SECTION("Parse JSON string with escapes '\\\\' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \\ ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == R"(abcdefghijklmnopqrstuvwxyz \ )");
  }
  SECTION("Parse JSON string with escapes '\\b' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \b ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \b ");
  }
  SECTION("Parse JSON string with escapes '\\/' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \/ ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz / ");
  }
  SECTION("Parse JSON string with escapes '\\f' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \f ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \f ");
  }
  SECTION("Parse JSON string with escapes '\\n' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \n ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \n ");
  }
  SECTION("Parse JSON string with escapes '\\r' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \r ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \r ");
  }
  SECTION("Parse JSON string with escapes '\\t' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz \t ");
  }
  SECTION("Parse JSON string with escapes '\\u0123' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0123 ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0123 " };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes '\\u0123 \\u0456' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0123 \u0456 ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0123 \u0456 " };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes  '\\uD834\\uDD1E' to buffer and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz  \uD834\uDD1E ")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz  \U0001D11E " };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escapes '\\u0005 \\u0004 \\u0003 \\u0002 \\u0001' to buffer and check value.",
    "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0005 \u0004 \u0003 \u0002 \u0001")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"abcdefghijklmnopqrstuvwxyz \u0005 \u0004 \u0003 \u0002 \u0001" };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  // SECTION("Parse JSON string with escapes '\\u0000' to buffer and expect exception.",
  //   "[JSON][Parse][Escapes]")
  // {
  //   BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u0000")" };
  //   REQUIRE_THROWS_AS(json.parse(jsonSource), JSON_Lib::Error);

  // }
  SECTION("Parse JSON string with multiple escapes '\\t\\n\\r' combined and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("a\tb\nc\r")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "a\tb\nc\r");
  }
  SECTION("Parse JSON string with escape '\\t' at start and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("\tabcdefghijklmnopqrstuvwxyz")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "\tabcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse JSON string with escape '\\n' at end and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz\n")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz\n");
  }
  SECTION("Parse JSON string containing only a single escape '\\n' and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("\n")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "\n");
  }
  SECTION(
    "Parse JSON string with escape '\\u00E9' (Latin small e with acute) and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("caf\u00E9")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"caf\u00E9" };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escape '\\u4E2D' (CJK character) and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("\u4E2D\u6587")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"\u4E2D\u6587" };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON string with escape '\\uFFFF' (highest BMP code point) and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("test \uFFFF end")" };
    json.parse(jsonSource);
    std::u8string expected{ u8"test \uFFFF end" };
    REQUIRE(NRef<String>(json.root()).value() == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Parse JSON object with escape sequences in its string value and check value.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"({"key":"line1\nline2\ttabbed"})" };
    json.parse(jsonSource);
    REQUIRE(isA<Object>(json.root()));
    REQUIRE(NRef<String>(json.root()["key"]).value() == "line1\nline2\ttabbed");
  }
  SECTION("Parse JSON array with escape sequences in string elements and check values.", "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"(["abc\tdef","ghi\njkl","mn\\op"])" };
    json.parse(jsonSource);
    REQUIRE(isA<Array>(json.root()));
    REQUIRE(NRef<String>(json.root()[0]).value() == "abc\tdef");
    REQUIRE(NRef<String>(json.root()[1]).value() == "ghi\njkl");
    REQUIRE(NRef<String>(json.root()[2]).value() == R"(mn\op)");
  }
  SECTION("Parse JSON string with unrecognized escape '\\q' and verify it is silently treated as 'q'.",
    "[JSON][Parse][Escapes]")
  {
    BufferSource jsonSource{ R"("abc \q def")" };
    json.parse(jsonSource);
    // '\q' is not a recognised JSON escape: the translator drops the '\' and keeps 'q'
    REQUIRE(NRef<String>(json.root()).value() == "abc q def");
  }
}

TEST_CASE("Check JSON parser throws on invalid escape sequences.", "[JSON][Parse][Escapes][Exception]")
{
  const JSON json;
  SECTION("Parse JSON string with incomplete '\\u' escape (3 hex digits) and expect ITranslator::Error.",
    "[JSON][Parse][Escapes][Exception]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \u012 ")" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), ITranslator::Error);
  }
  SECTION("Parse JSON string with '\\u' followed by non-hex digits and expect ITranslator::Error.",
    "[JSON][Parse][Escapes][Exception]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \uXYZW ")" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), ITranslator::Error);
  }
  SECTION("Parse JSON string with lone high surrogate '\\uD800' (no low surrogate) and expect ITranslator::Error.",
    "[JSON][Parse][Escapes][Exception]")
  {
    BufferSource jsonSource{ R"("abcdefghijklmnopqrstuvwxyz \uD800 ")" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), ITranslator::Error);
  }
}