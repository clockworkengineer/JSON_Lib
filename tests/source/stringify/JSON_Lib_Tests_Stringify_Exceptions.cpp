#include "JSON_Lib_Tests.hpp"

// ====================================
// Stringification generated exceptions
// ====================================
TEST_CASE("Check JSON stringify generated exceptions.", "[JSON][Stringify][Exception]")
{
  const JSON json;
  SECTION("Stringify has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
  }
}