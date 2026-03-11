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
    const std::string generatedFileName{ generateRandomFileName() };
    std::string jsonFileBuffer{ JSON::fromFile(testFileName) };
    BufferSource jsonSource{ jsonFileBuffer };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == stripWhiteSpace(jsonFileBuffer));
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Parse from file source and stringify to buffer gives same result as parsing from buffer.",
    "[JSON][Stringify][Buffer]")
  {
    const std::string testFileName{ prefixTestDataPath(testFile) };
    const std::string jsonFileBuffer{ JSON::fromFile(testFileName) };
    // Parse from BufferSource
    BufferDestination bufferResult;
    json.parse(BufferSource{ jsonFileBuffer });
    json.stringify(bufferResult);
    // Parse from FileSource
    BufferDestination fileResult;
    json.parse(FileSource{ testFileName });
    json.stringify(fileResult);
    REQUIRE(bufferResult.toString() == fileResult.toString());
  }
  SECTION("Stringify output is idempotent (re-parse and re-stringify produces identical output).",
    "[JSON][Stringify][Buffer]")
  {
    const std::string jsonFileBuffer{ JSON::fromFile(prefixTestDataPath(testFile)) };
    // First pass
    BufferDestination first;
    json.parse(BufferSource{ jsonFileBuffer });
    json.stringify(first);
    // Second pass: re-parse the stringified result
    BufferDestination second;
    json.parse(BufferSource{ first.toString() });
    json.stringify(second);
    REQUIRE(first.toString() == second.toString());
  }
  SECTION("Stringify output contains no leading or trailing whitespace.", "[JSON][Stringify][Buffer]")
  {
    const std::string jsonFileBuffer{ JSON::fromFile(prefixTestDataPath(testFile)) };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ jsonFileBuffer });
    json.stringify(jsonDestination);
    const std::string result{ jsonDestination.toString() };
    REQUIRE_FALSE(result.empty());
    REQUIRE(result.front() != ' ');
    REQUIRE(result.back() != ' ');
  }
}
