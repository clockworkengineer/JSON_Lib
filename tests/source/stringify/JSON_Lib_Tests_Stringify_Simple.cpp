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
  SECTION("Stringify a boolean (false) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "false" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a negative integer (-98345) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "-98345" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify an integer (0) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "0" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a floating point (55.6667) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "55.6667" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a floating point (-55.6667) to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ "-55.6667" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify an empty string to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ R"("")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Stringify a string with spaces to buffer.", "[JSON][Stringify][Simple][Buffer][Validate]")
  {
    const std::string expected{ R"("hello world")" };
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
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    ;
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a boolean (true) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "true" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a number (98345) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "98345" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a null to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "null" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a boolean (false) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "false" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a negative integer (-98345) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "-98345" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify an integer (0) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "0" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a floating point (55.6667) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "55.6667" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify a floating point (-55.6667) to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ "-55.6667" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION("Stringify an empty string to file.", "[JSON][Stringify][Simple][File][Validate]")
  {
    const std::string expected{ R"("")" };
    const std::string generatedFileName{ prefixTestDataPath(generateRandomFileName()) };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
}