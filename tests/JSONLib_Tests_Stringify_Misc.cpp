//
// Unit Tests: JSON
//
// Description: JSON stringification miscellaneous unit
// tests for JSON class using the Catch2 test framework.
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
// ================================================
// Stringification to file and validation of result
// ================================================
TEST_CASE("Stringify to a file and check result.",
          "[JSON][Stringify][File][Validate]") {
  const JSON json;
  const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
  SECTION("Stringify object to file and check value",
          "[JSON][Stringify][File][Validate]") {
    const std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{generatedFileName};
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION("Stringify array to file and check value",
          "[JSON][Stringify][File][Validate]") {
    const std::string expected{"[999,\"Time\",null,true]"};
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{generatedFileName};
    json.parse(BufferSource{expected});
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
}
// =================================
// Parsing and stringification files
// =================================
TEST_CASE("JSON object for stringification of a list of example JSON files.",
          "[JSON][Stringify]") {
  const JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Stringify to  buffer and check value", "[JSON][Stringify][Buffer]") {
    const std::string jsonFileBuffer{
        readFromFile(prefixTestDataPath(testFile))};
    BufferSource jsonSource{jsonFileBuffer};
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() ==
            stripWhiteSpace(json, jsonFileBuffer));
  }
  SECTION("Stringify to file and check value", "[JSON][Stringify][File]") {
    const std::string testFileName{prefixTestDataPath(testFile)};
    const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::remove(generatedFileName);
    std::string jsonFileBuffer{readFromFile(testFileName)};
    BufferSource jsonSource{jsonFileBuffer};
    FileDestination jsonDestination{generatedFileName};
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) ==
            stripWhiteSpace(json, jsonFileBuffer));
  }
}
