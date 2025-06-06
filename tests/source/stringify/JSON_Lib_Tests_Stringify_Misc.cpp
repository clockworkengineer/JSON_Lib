#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON stringification of a list of example JSON files.", "[JSON][Stringify]")
{
  const JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Stringify to  buffer and check value.", "[JSON][Stringify][Buffer]")
  {
    const std::string jsonFileBuffer{ JSON::fromFile(prefixTestDataPath(testFile)) };
    BufferSource jsonSource{ jsonFileBuffer };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == stripWhiteSpace(jsonFileBuffer));
  }
  SECTION("Stringify to file and check value.", "[JSON][Stringify][File]")
  {
    const std::string testFileName{ prefixTestDataPath(testFile) };
    const std::string generatedFileName{ prefixTestDataPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    std::string jsonFileBuffer{ JSON::fromFile(testFileName) };
    BufferSource jsonSource{ jsonFileBuffer };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == stripWhiteSpace(jsonFileBuffer));
  }
}
