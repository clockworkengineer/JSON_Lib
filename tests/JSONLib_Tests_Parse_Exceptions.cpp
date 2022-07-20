//
// Unit Tests: JSON
//
// Description: JSON parse exception unit tests for JSON class
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
// ===========================
// Parser Generated Exceptions
// ===========================
TEST_CASE("Parse generated exceptions.", "[JSON][Parse][Exceptions]") {
  const JSON json;
  SECTION("Parse missing terminating '\"' in string",
          "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{ \"one\" : \"Apple }"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse number with starting with invalid character",
          "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{ \"one\" : z19034}"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse object with invalid value field (number).",
          "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{ \"one\" : 18987u3 }"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse object with missing value field.",
          "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{ \"one\" : }"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse object with missing key field.", "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{  : 89012 }"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse object with missing closing '}'.",
          "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{  \"one\" : 18987"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
  SECTION("Parse an nested objects ({ {} }) ", "[JSON][Parse][Exceptions]") {
    BufferSource jsonSource{"{{}}"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource),
                        "JSON Error: Syntax error detected.");
  }
}
