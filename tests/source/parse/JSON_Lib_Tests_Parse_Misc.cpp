#include "JSON_Lib_Tests.hpp"
#include "Default_Parser.hpp"

TEST_CASE("Check JSON parsing of a list of example JSON files.", "[JSON][Parse][Examples]")
{
  JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Parse from buffer.", "[JSON][Parse][Examples][Buffer]")
  {
    BufferSource jsonSource{ JSON::fromFile(prefixTestDataPath(testFile)) };
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE_FALSE(!(isA<Object>(json.root()) || (isA<Array>(json.root()))));
  }
  SECTION("Parse from file directly.", "[JSON][Parse][Examples][File]")
  {
    FileSource jsonSource{ prefixTestDataPath(testFile) };
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE_FALSE(!(isA<Object>(json.root()) || (isA<Array>(json.root()))));
  }
}
TEST_CASE("Check string overflow handling.", "[Bencode][String][Overflow]")
{
  const JSON json;
  SECTION("Default max string 16K", "[Bencode][Parse][String]") { REQUIRE(String::getMaxStringLength() == 16 * 1024); }
  SECTION("Parse a string larger than the max allowed length of 16K", "[Bencode][Parse][String]")
  {
    REQUIRE_THROWS_WITH(json.parse(BufferSource{ "[\"" + std::string(String::getMaxStringLength() + 1, '0') + "\"]" }),
      "JSON Syntax Error: String size exceeds maximum allowed size.");
  }
}
TEST_CASE("Check parse depth handling.", "[JSON][Parse][Depth]")
{
  const JSON json;
  SECTION("Parse 2 nested list.", "[Bencode][Parse][Depth]") { REQUIRE_NOTHROW(json.parse(BufferSource("[[]]"))); }
  SECTION("Parse 11 nested list.", "[Bencode][Parse][Depth]")
  {
    REQUIRE_THROWS_WITH(
      json.parse(BufferSource("[[[[[[[[[[[]]]]]]]]]]]")), "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Get default maximum parser depth.", "[Bencode][Parse][Depth]")
  {
    REQUIRE(Default_Parser::getMaxParserDepth() == 10);
  }
  SECTION("Set default maximum parser depth.", "[Bencode][Parse][Depth]")
  {
    Default_Parser::setMaxParserDepth(20);
    REQUIRE(Default_Parser::getMaxParserDepth() == 20);
    Default_Parser::setMaxParserDepth(Default_Parser::kDefaultMaxParserDepth);
  }
  SECTION("Set default maximum parser depth and check new value works.", "[Bencode][Parse][Depth]")
  {
    Default_Parser::setMaxParserDepth(20);
    REQUIRE(Default_Parser::getMaxParserDepth() == 20);
    REQUIRE_NOTHROW(json.parse(BufferSource("[[[[[[[[[[[]]]]]]]]]]]")));
    Default_Parser::setMaxParserDepth(Default_Parser::kDefaultMaxParserDepth);
  }
}
TEST_CASE("Check parse depth boundary conditions.", "[JSON][Parse][Depth][Boundary]")
{
  const JSON json;
  SECTION("Parse array nested 9 levels deep (one below max) succeeds.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_NOTHROW(json.parse(BufferSource("[[[[[[[[[]]]]]]]]]]")));
  }
  SECTION("Parse array nested exactly at max depth (10 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_THROWS_WITH(json.parse(BufferSource("[[[[[[[[[[]]]]]]]]]]")),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Parse object nested 9 levels deep (one below max) succeeds.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_NOTHROW(json.parse(BufferSource(R"({"a":{"b":{"c":{"d":{"e":{"f":{"g":{"h":{}}}}}}}}})")));
  }
  SECTION("Parse object nested exactly at max depth (10 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_THROWS_WITH(json.parse(BufferSource(R"({"a":{"b":{"c":{"d":{"e":{"f":{"g":{"h":{"i":{}}}}}}}}}}})")),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Parse object nested beyond max depth (11 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_THROWS_WITH(json.parse(BufferSource(R"({"a":{"b":{"c":{"d":{"e":{"f":{"g":{"h":{"i":{"j":{}}}}}}}}}}})")),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("kDefaultMaxParserDepth constant equals 10.", "[JSON][Parse][Depth][Boundary]") {
    REQUIRE(Default_Parser::kDefaultMaxParserDepth == 10);
  }
}
TEST_CASE("Check string max length can be configured.", "[JSON][Parse][String][MaxLength]")
{
  const JSON json;
  SECTION("kDefMaxStringLength constant equals 16K.", "[JSON][Parse][String][MaxLength]") {
    REQUIRE(String::kDefMaxStringLength == 16 * 1024);
  }
  SECTION("Set smaller max string length and parse string exactly at that limit succeeds.", "[JSON][Parse][String][MaxLength]") {
    String::setMaxStringLength(100);
    REQUIRE_NOTHROW(json.parse(BufferSource{ "[\"" + std::string(100, 'x') + "\"]" }));
    String::setMaxStringLength(String::kDefMaxStringLength);
  }
  SECTION("Set smaller max string length and parse string exceeding it throws.", "[JSON][Parse][String][MaxLength]") {
    String::setMaxStringLength(100);
    REQUIRE_THROWS_WITH(json.parse(BufferSource{ "[\"" + std::string(101, 'x') + "\"]" }),
      "JSON Syntax Error: String size exceeds maximum allowed size.");
    String::setMaxStringLength(String::kDefMaxStringLength);
  }
  SECTION("Restore max string length to default and verify.", "[JSON][Parse][String][MaxLength]") {
    String::setMaxStringLength(50);
    String::setMaxStringLength(String::kDefMaxStringLength);
    REQUIRE(String::getMaxStringLength() == static_cast<uint64_t>(16 * 1024));
  }
}

