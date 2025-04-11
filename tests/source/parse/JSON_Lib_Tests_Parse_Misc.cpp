#include "JSON_Lib_Tests.hpp"
#include "JSON_Parser.hpp"

TEST_CASE("Check JSON parsing of a list of example JSON files.", "[JSON][Parse][Examples]")
{
  JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Parse from buffer.", "[JSON][Parse][Examples][Buffer]")
  {
    BufferSource jsonSource{ JSON::fromFile(prefixPath(testFile)) };
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE_FALSE(!(isA<Object>(json.root()) || (isA<Array>(json.root()))));
  }
  SECTION("Parse from file directly.", "[JSON][Parse][Examples][File]")
  {
    FileSource jsonSource{ prefixPath(testFile) };
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE_FALSE(!(isA<Object>(json.root()) || (isA<Array>(json.root()))));
  }

}
TEST_CASE("Check parse depth handling.", "[JSON][Parse][Depth]")
{
  const JSON json;
  SECTION("Parse 2 nested list.", "[Bencode][Parse][Depth]") {
    REQUIRE_NOTHROW(json.parse(BufferSource("[[]]")));
  }
  SECTION("Parse 11 nested list.", "[Bencode][Parse][Depth]") {
    REQUIRE_THROWS_WITH(json.parse(BufferSource("[[[[[[[[[[[]]]]]]]]]]]")),"JSON Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Get default maximum parser depth.", "[Bencode][Parse][Depth]") {
    REQUIRE(JSON_Parser::getMaxParserDepth()==10);
  }
  SECTION("Set default maximum parser depth.", "[Bencode][Parse][Depth]") {
    JSON_Parser::setMaxParserDepth(20);
    REQUIRE(JSON_Parser::getMaxParserDepth()==20);
  }
  SECTION("Set default maximum parser depth and check new value works.", "[Bencode][Parse][Depth]") {
    JSON_Parser::setMaxParserDepth(20);
    REQUIRE(JSON_Parser::getMaxParserDepth()==20);
    REQUIRE_NOTHROW(json.parse(BufferSource("[[[[[[[[[[[]]]]]]]]]]]")));
  }
}

