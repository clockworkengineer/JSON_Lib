//
// Unit Tests: JSON_Lib_Tests_Stringify_Simple
//
// Description: JSON stringification of simple types unit
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
// ========================================================
// Stringification of simple types to buffer and validation
// ========================================================
TEST_CASE("Check JSON stringification of simple types to buffer and check values.",
  "[JSON][Stringify][Simple][Buffer][Validate]")
{
  const JSON json;
  SECTION(
    "Stringify a string (Test string) to buffer and check its value.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ R"("Test string.")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a boolean (true) to buffer and check its value.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "true" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a number (98345) to buffer and check its value.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "98345" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Stringify a null to buffer  and check its value.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "null" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}
// ======================================================
// Stringification of simple types to file and validation
// ======================================================
TEST_CASE("Check JSON stringification of simple types to file and check values.",
  "[JSON][Stringify][Simple][File][Validate]")
{
  const JSON json;
  SECTION("Stringify a string (Test string) to file and check its value.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ R"("Test string.")" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(crlfTolf(JSON::fromFile(generatedFileName)) == expected);
  }
  SECTION("Stringify a boolean (true) to file and check its value.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "true" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(crlfTolf(JSON::fromFile(generatedFileName)) == expected);
  }
  SECTION("Stringify a number (98345) to file and check its value.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "98345" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(crlfTolf(JSON::fromFile(generatedFileName)) == expected);
  }
  SECTION("Stringify a null to file and check its value.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "null" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(crlfTolf(JSON::fromFile(generatedFileName)) == expected);
  }
}