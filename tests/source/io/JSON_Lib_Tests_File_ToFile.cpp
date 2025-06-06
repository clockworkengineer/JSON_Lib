#include "JSON_Lib_Tests.hpp"

TEST_CASE("Checks for toFile() api.", "[JSON][ToFile]")
{
  SECTION("Check that toFile() works with UTF8.", "[JSON][ToFile][UTF8]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF8BOM.", "[JSON][ToFile][UTF8BOM]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf8BOM);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BE.", "[JSON][ToFile][UTF16BE]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16BE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BLE.", "[JSON][ToFile][UTF16LE]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16LE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF32BE.", "[JSON][ToFile][UTF32BE]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32BE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
  SECTION("Check that toFile() works with UTF32LE.", "[JSON][ToFile][UTF32LE]")
  {
    std::string testFile{ prefixTestDataPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32LE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
}
