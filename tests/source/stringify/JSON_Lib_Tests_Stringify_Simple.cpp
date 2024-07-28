#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON stringification of simple types to buffer and check values.",
  "[JSON][Stringify][Simple][Buffer][Validate]")
{
  const JSON json;
  SECTION("Stringify a string (abcdefghijklmnopqrstuvwxyz) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a boolean (true) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "true" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a number (98345) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "98345" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a null to buffer .", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "null" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
}

TEST_CASE("Check JSON stringification of simple types to file and check values.",
  "[JSON][Stringify][Simple][File][Validate]")
{
  const JSON json;
  SECTION("Stringify a string (abcdefghijklmnopqrstuvwxyz) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz")" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Stringify a boolean (true) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "true" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Stringify a number (98345) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "98345" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Stringify a null to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "null" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
}