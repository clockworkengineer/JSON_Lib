//
// Unit Tests: JSON
//
// Description: JSON parse miscellaneous unit tests for JSON class
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
// ========================
// Parse Example JSON Files
// ========================
TEST_CASE("JSON object for parse of a list of example JSON files.",
          "[JSON][Parse][Examples]") {
  const JSON json;

  TEST_FILE_LIST(testFile);
  SECTION("Parse from buffer", "[JSON][Parse][Examples][Buffer]") {
    BufferSource jsonSource{readFromFile(prefixPath(testFile))};
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE((((json.root()).getType() == JNodeType::object) ||
             ((json.root()).getType() == JNodeType::array)));
  }
  SECTION("Parse from file directly", "[JSON][Parse][Examples][File]") {
    FileSource jsonSource{prefixPath(testFile)};
    REQUIRE_NOTHROW(json.parse(jsonSource));
    jsonSource.reset();
    json.parse(jsonSource);
    REQUIRE((((json.root()).getType() == JNodeType::object) ||
             ((json.root()).getType() == JNodeType::array)));
  }
}
