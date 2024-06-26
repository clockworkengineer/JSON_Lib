//
// Unit Tests: JSON_Lib_Tests_Parse_Misc
//
// Description: JSON parse miscellaneous unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// ========================
// Parse Example JSON Files
// ========================
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
