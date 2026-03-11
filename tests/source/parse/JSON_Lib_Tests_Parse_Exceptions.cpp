#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check parser generated exceptions.", "[JSON][Parse][Exception]")
{
  const JSON json;
  SECTION("Parse missing terminating '\"' in string.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "one" : "Apple })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 19]: Missing closing '\"' on string.");
  }
  SECTION("Parse number with starting with invalid character.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "one" : z19034})" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
      "JSON Syntax Error [Line: 1 Column: 11]: Missing String, Number, Boolean, Array, Object or Null.");
  }
  SECTION("Parse object with invalid value field (number).", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "one" : 18987u3 })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 18]: Invalid numeric value.");
  }
  SECTION("Parse object with missing value field.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "one" : })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
      "JSON Syntax Error [Line: 1 Column: 11]: Missing String, Number, Boolean, Array, Object or Null.");
  }
  SECTION("Parse object with missing key field.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({  : 89012 })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 4]: Missing opening '\"' on string.");
  }
  SECTION("Parse object with an invalid boolean value.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "key" : trrue })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 13]: Invalid boolean value.");
  }
  SECTION("Parse object with an invalid nul; value.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "key" : nul })" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 14]: Invalid null value.");
  }
  SECTION("Parse object with missing ':'' between key and value fields.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({ "key" 4444})" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 9]: Missing ':' in key value pair.");
  }
  SECTION("Parse object with missing closing '}'.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({  "one" : 18987)" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 17]: Missing closing '}' in object definition.");
  }
  SECTION("Parse array with missing closing ']'.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"([1,2,3,4,5,6)" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 13]: Missing closing ']' in array definition.");
  }
  SECTION("Parse an nested objects ({ {} }) .", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ "{{}}" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 2]: Missing opening '\"' on string.");
  }
  SECTION("Parse array with trailing comma ([1,2,3,]).", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"([1,2,3,])" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
      "JSON Syntax Error [Line: 1 Column: 8]: Missing String, Number, Boolean, Array, Object or Null.");
  }
  SECTION(R"(Parse object with trailing comma ({"a":1,}).)", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({"a":1,})" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 8]: Missing opening '\"' on string.");
  }
  SECTION("Parse array with double comma ([1,,2]).", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"([1,,2])" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
      "JSON Syntax Error [Line: 1 Column: 4]: Missing String, Number, Boolean, Array, Object or Null.");
  }
  SECTION(R"(Parse object with non-string key ({123:"val"}).)", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ R"({123:"val"})" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(
      json.parse(jsonSource), "JSON Syntax Error [Line: 1 Column: 2]: Missing opening '\"' on string.");
  }
  SECTION("Parse JSON exceeding maximum parser depth.", "[JSON][Parse][Exception]")
  {
    Default_Parser::setMaxParserDepth(3);
    BufferSource jsonSource{ R"([[[[1]]]])" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error: Maximum parser depth exceeded.");
    Default_Parser::setMaxParserDepth(Default_Parser::kDefaultMaxParserDepth);
  }
  SECTION("Parse multi-line JSON with a syntax error on line 2.", "[JSON][Parse][Exception]")
  {
    BufferSource jsonSource{ "{\n\"key\" : trrue }" };
    REQUIRE_THROWS_AS(json.parse(jsonSource), SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Syntax Error [Line: 2 Column: 12]: Invalid boolean value.");
  }
}
