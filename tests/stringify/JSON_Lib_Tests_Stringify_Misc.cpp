//
// Unit Tests: JSON_Lib_Tests_Stringify_Misc
//
// Description: JSON stringification miscellaneous unit
// tests for JSON class using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSON_Lib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSON_Lib;
// ==========
// Test cases
// ==========
// ====================================
// Parsing and stringification of files
// ====================================
TEST_CASE("Check JSON stringification of a list of example JSON files.", "[JSON][Stringify]")
{
  const JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Stringify to  buffer and check value.", "[JSON][Stringify][Buffer]")
  {
    const std::string jsonFileBuffer{ readFromFile(prefixPath(testFile)) };
    BufferSource jsonSource{ jsonFileBuffer };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == stripWhiteSpace(jsonFileBuffer));
  }
  SECTION("Stringify to file and check value.", "[JSON][Stringify][File]")
  {
    const std::string testFileName{ prefixPath(testFile) };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    std::string jsonFileBuffer{ readFromFile(testFileName) };
    BufferSource jsonSource{ jsonFileBuffer };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(readFromFile(generatedFileName) == stripWhiteSpace(jsonFileBuffer));
  }
}
