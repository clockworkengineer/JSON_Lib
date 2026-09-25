#include "catch2/catch_all.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Error.hpp"

using namespace JSON_Lib;

TEST_CASE("UTF-8 to UTF-16 and back - ASCII and basic characters", "[Converter][UTF]")
{
  std::string original = "Hello, World! 12345 [] {}";
  std::u16string u16 = toUtf16(original);
  REQUIRE(u16.size() == original.size());
  REQUIRE(toUtf8(u16) == original);
}

TEST_CASE("UTF-8 to UTF-16 and back - Multibyte 2-byte characters", "[Converter][UTF]")
{
  // Latin accented, Greek, Cyrillic
  std::string original = "café, naïve, résumé, Ελληνικά, Русский";
  std::u16string u16 = toUtf16(original);
  REQUIRE(toUtf8(u16) == original);
}

TEST_CASE("UTF-8 to UTF-16 and back - Multibyte 3-byte characters", "[Converter][UTF]")
{
  // CJK, Devanagari, symbols
  std::string original = "日本語, 中文, 한국어, हिन्दी, €";
  std::u16string u16 = toUtf16(original);
  REQUIRE(toUtf8(u16) == original);
}

TEST_CASE("UTF-8 to UTF-16 and back - Multibyte 4-byte surrogate pairs", "[Converter][UTF]")
{
  // Emojis, Musical symbol G-clef (U+1D11E)
  std::string original = "𝄞 🚀 🍕 🎉";
  std::u16string u16 = toUtf16(original);
  // Each 4-byte UTF-8 character becomes 2 char16_t code units (surrogate pair)
  REQUIRE(toUtf8(u16) == original);
}

TEST_CASE("UTF-8 to UTF-16 - Rejection of embedded null characters", "[Converter][UTF]")
{
  std::string withNull = "hello";
  withNull.push_back('\0');
  withNull += "world";

  REQUIRE_THROWS_AS(toUtf16(withNull), Error);
}

TEST_CASE("UTF-16 to UTF-8 - Rejection of embedded null characters", "[Converter][UTF]")
{
  std::u16string withNull = u"hello";
  withNull.push_back(u'\0');
  withNull += u"world";

  REQUIRE_THROWS_AS(toUtf8(withNull), Error);
}

TEST_CASE("toUtf8 single char16_t overload", "[Converter][UTF]")
{
  char16_t ascii = u'A';
  REQUIRE(toUtf8(ascii) == "A");

  char16_t euro = u'\u20AC'; // €
  REQUIRE(toUtf8(euro) == "€");
}

TEST_CASE("UTF-8 conversion - Error handling on malformed UTF-8", "[Converter][UTF]")
{
  // Truncated 2-byte sequence
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xC2"), Error);

  // Invalid continuation byte
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xC2\x20"), Error);

  // Overlong 2-byte sequence
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xC0\xAF"), Error);

  // Truncated 3-byte sequence
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xE0\xA0"), Error);

  // Surrogate codepoint in UTF-8 (U+D800: 0xED 0xA0 0x80)
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xED\xA0\x80"), Error);

  // Truncated 4-byte sequence
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xF0\x90\x80"), Error);

  // Codepoint out of Unicode range (> 0x10FFFF)
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xF4\x90\x80\x80"), Error);

  // Invalid leading byte
  REQUIRE_THROWS_AS(JSON_Lib::toUtf16("\xFF"), Error);
}

TEST_CASE("UTF-16 conversion - Error handling on malformed UTF-16", "[Converter][UTF]")
{
  // Unpaired high surrogate
  std::u16string unpairedHigh = { 0xD800 };
  REQUIRE_THROWS_AS(JSON_Lib::toUtf8(unpairedHigh), Error);

  // High surrogate followed by non-surrogate
  std::u16string invalidPair = { 0xD800, 0x0041 };
  REQUIRE_THROWS_AS(JSON_Lib::toUtf8(invalidPair), Error);

  // Unpaired low surrogate
  std::u16string unpairedLow = { 0xDC00 };
  REQUIRE_THROWS_AS(JSON_Lib::toUtf8(unpairedLow), Error);
}
