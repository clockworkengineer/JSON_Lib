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
  JSON json;
  SECTION("Default max string 16K", "[Bencode][Parse][String]") { REQUIRE(getDefaultStringLength() == 16 * 1024); }
  SECTION("Parse a string larger than the max allowed length of 16K", "[Bencode][Parse][String]")
  {
    REQUIRE_THROWS_WITH(json.parse(BufferSource{ "[\"" + std::string(getDefaultStringLength() + 1, '0') + "\"]" }),
      "JSON Syntax Error: String size exceeds maximum allowed size.");
  }
}
TEST_CASE("Check parse depth handling.", "[JSON][Parse][Depth]")
{
  JSON json;
  SECTION("Parse 2 nested list.", "[Bencode][Parse][Depth]") { REQUIRE_NOTHROW(json.parse(BufferSource("[[]]"))); }
  SECTION("Parse 129 nested arrays (exceeds default depth of 128).", "[Bencode][Parse][Depth]")
  {
    REQUIRE_THROWS_WITH(
      json.parse(BufferSource(std::string(129, '[') + std::string(129, ']'))),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Get default maximum parser depth.", "[Bencode][Parse][Depth]")
  {
    REQUIRE(json.getMaxParserDepth() == 128);
  }
  SECTION("Set default maximum parser depth.", "[Bencode][Parse][Depth]")
  {
    const ScopedMaxDepth scopedDepth(json, 20);
    REQUIRE(json.getMaxParserDepth() == 20);
  }
  SECTION("Set default maximum parser depth and check new value works.", "[Bencode][Parse][Depth]")
  {
    const ScopedMaxDepth scopedDepth(json, 20);
    REQUIRE(json.getMaxParserDepth() == 20);
    REQUIRE_NOTHROW(json.parse(BufferSource("[[[[[[[[[[[]]]]]]]]]]]")));
  }
}
TEST_CASE("Check parse depth boundary conditions.", "[JSON][Parse][Depth][Boundary]")
{
  JSON json;
  SECTION("Parse array nested 127 levels deep (one below max) succeeds.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_NOTHROW(json.parse(BufferSource(std::string(127, '[') + std::string(127, ']'))));
  }
  SECTION("Parse array nested exactly at max depth (128 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_THROWS_WITH(
      json.parse(BufferSource(std::string(128, '[') + std::string(128, ']'))),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Parse array nested beyond max depth (129 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    REQUIRE_THROWS_WITH(
      json.parse(BufferSource(std::string(129, '[') + std::string(129, ']'))),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Parse object nested 126 levels deep (one below max) succeeds.", "[JSON][Parse][Depth][Boundary]")
  {
    std::string deepObj;
    for (int i = 0; i < 126; i++) deepObj += "{\"a\":";
    deepObj += "{}";
    for (int i = 0; i < 126; i++) deepObj += "}";
    REQUIRE_NOTHROW(json.parse(BufferSource(deepObj)));
  }
  SECTION("Parse object nested exactly at max depth (127 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    std::string deepObj;
    for (int i = 0; i < 127; i++) deepObj += "{\"a\":";
    deepObj += "{}";
    for (int i = 0; i < 127; i++) deepObj += "}";
    REQUIRE_THROWS_WITH(json.parse(BufferSource(deepObj)),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Parse object nested beyond max depth (128 levels) throws.", "[JSON][Parse][Depth][Boundary]")
  {
    std::string deepObj;
    for (int i = 0; i < 128; i++) deepObj += "{\"a\":";
    deepObj += "{}";
    for (int i = 0; i < 128; i++) deepObj += "}";
    REQUIRE_THROWS_WITH(json.parse(BufferSource(deepObj)),
      "JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("kDefaultMaxParserDepth constant equals 128.", "[JSON][Parse][Depth][Boundary]") {
    REQUIRE(Default_Parser::kDefaultMaxParserDepth == 128);
  }
}
TEST_CASE("Check string max length can be configured.", "[JSON][Parse][String][MaxLength]")
{
  JSON json;
  SECTION("kDefMaxStringLength constant equals 16K.", "[JSON][Parse][String][MaxLength]") {
    REQUIRE(String::kDefMaxStringLength == 16 * 1024);
  }
  SECTION("Set smaller default max string length and parse string exactly at that limit succeeds.", "[JSON][Parse][String][MaxLength]") {
    const ScopedMaxStringLength scopedStringLength(100);
    REQUIRE_NOTHROW(json.parse(BufferSource{ "[\"" + std::string(100, 'x') + "\"]" }));
  }
  SECTION("Set smaller default max string length and parse string exceeding it throws.", "[JSON][Parse][String][MaxLength]") {
    const ScopedMaxStringLength scopedStringLength(100);
    REQUIRE_THROWS_WITH(json.parse(BufferSource{ "[\"" + std::string(101, 'x') + "\"]" }),
      "JSON Syntax Error: String size exceeds maximum allowed size.");
  }
  SECTION("Restore max string length to default and verify.", "[JSON][Parse][String][MaxLength]") {
    const ScopedMaxStringLength scopedStringLength(50);
    setDefaultStringLength(String::kDefMaxStringLength);
    REQUIRE(getDefaultStringLength() == static_cast<uint64_t>(16 * 1024));
  }
}

