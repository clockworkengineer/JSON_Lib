#include "JSON_Lib_Tests.hpp"

TEST_CASE("Checks for fromFile() api.", "[JSON][FromFile]")
{
  SECTION("Check that fromFile() works with UTF8.", "[JSON][FromFile][UTF8]")
  {
    std::string testFile{ prefixTestDataPath("testfile021.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF8BOM.", "[JSON][FromFile][UTF8BOM]")
  {
    std::string testFile{ prefixTestDataPath("testfile022.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16BE.", "[JSON][FromFile][UTF16BE]")
  {
    std::string testFile{ prefixTestDataPath("testfile023.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16LE.", "[JSON][FromFile][UTF16LE]")
  {
    std::string testFile{ prefixTestDataPath("testfile024.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF32BE.", "[JSON][FromFile][UTF32BE]")
  {
    std::string testFile{ prefixTestDataPath("testfile025.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF32LE.", "[JSON][FromFile][UTF32LE]")
  {
    std::string testFile{ prefixTestDataPath("testfile026.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF16LE and leading spaces.", "[JSON][FromFile][UTF16LE][Whitespace]")
  {
    std::string testFile{ prefixTestDataPath("testfile027.json") };
    std::string expected{ R"(   [true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
}

