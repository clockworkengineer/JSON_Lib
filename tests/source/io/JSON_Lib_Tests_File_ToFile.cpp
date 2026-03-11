#include "JSON_Lib_Tests.hpp"

TEST_CASE("Checks for toFile() api.", "[JSON][ToFile]")
{
  SECTION("Check that toFile() works with UTF8.", "[JSON][ToFile][UTF8]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected);
    REQUIRE(JSON::fromFile(testFile) == expected);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with UTF8BOM.", "[JSON][ToFile][UTF8BOM]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf8BOM);
    REQUIRE(JSON::fromFile(testFile) == expected);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with UTF16BE.", "[JSON][ToFile][UTF16BE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16BE);
    REQUIRE(JSON::fromFile(testFile) == expected);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with UTF16LE.", "[JSON][ToFile][UTF16LE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16LE);
    REQUIRE(JSON::fromFile(testFile) == expected);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() overwrites an existing file.", "[JSON][ToFile][Overwrite]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string first{ R"({"key":"first"})" };
    std::string second{ R"({"key":"second"})" };
    JSON::toFile(testFile, first);
    JSON::toFile(testFile, second);
    REQUIRE(JSON::fromFile(testFile) == second);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() writes correct BOM for UTF8BOM.", "[JSON][ToFile][BOM][UTF8BOM]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf8BOM);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf8BOM);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() writes correct BOM for UTF16BE.", "[JSON][ToFile][BOM][UTF16BE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf16BE);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf16BE);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() writes correct BOM for UTF16LE.", "[JSON][ToFile][BOM][UTF16LE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf16LE);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf16LE);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with an empty string.", "[JSON][ToFile][Empty]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    JSON::toFile(testFile, "");
    REQUIRE(JSON::fromFile(testFile).empty());
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with a JSON object.", "[JSON][ToFile][Object]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"({"City":"Southampton","Population":500000})" };
    JSON::toFile(testFile, expected);
    REQUIRE(JSON::fromFile(testFile) == expected);
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with UTF32BE.", "[JSON][ToFile][UTF32BE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32BE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
    std::filesystem::remove(testFile);
  }
  SECTION("Check that toFile() works with UTF32LE.", "[JSON][ToFile][UTF32LE]")
  {
    std::string testFile{ prefixTestDataPath(generateRandomFileName()) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32LE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
    std::filesystem::remove(testFile);
  }
}
