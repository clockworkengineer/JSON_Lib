#include "JSON_Lib_Tests.hpp"

// =============
// GetFileFormat
// =============
TEST_CASE("Checks for getFileFormat() api.", "[JSON][GetFileFormat]")
{
  SECTION("Check that getFileFormat() works with UTF8.", "[JSON][GetFileFormat][UTF8]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile021.json")) == JSON::Format::utf8);
  }
  SECTION("Check that getFileFormat() works with UTF8BOM.", "[JSON][GetFileFormat][UTF8BOM]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile022.json")) == JSON::Format::utf8BOM);
  }
  SECTION("Check that getFileFormat() works with UTF16BE.", "[JSON][GetFileFormat][UTF16BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile023.json")) == JSON::Format::utf16BE);
  }
  SECTION("Check that getFileFormat() works with UTF16LE.", "[JSON][GetFileFormat][UTF16LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile024.json")) == JSON::Format::utf16LE);
  }
  SECTION("Check that getFileFormat() works with UTF32BE.", "[JSON][GetFileFormat][UTF32BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile025.json")) == JSON::Format::utf32BE);
  }
  SECTION("Check that getFileFormat() works with UTF32LE.", "[JSON][GetFileFormat][UTF32LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile026.json")) == JSON::Format::utf32LE);
  }
}
// ========
// FromFile
// ========
TEST_CASE("Checks for fromFile() api.", "[JSON][FromFile]")
{
  SECTION("Check that fromFile() works with UTF8.", "[JSON][FromFile][UTF8]")
  {
    std::string testFile{ prefixPath("testfile021.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF8BOM.", "[JSON][FromFile][UTF8BOM]")
  {
    std::string testFile{ prefixPath("testfile022.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16BE.", "[JSON][FromFile][UTF16BE]")
  {
    std::string testFile{ prefixPath("testfile023.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16LE.", "[JSON][FromFile][UTF16LE]")
  {
    std::string testFile{ prefixPath("testfile024.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF32BE.", "[JSON][FromFile][UTF32BE]")
  {
    std::string testFile{ prefixPath("testfile025.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF32LE.", "[JSON][FromFile][UTF32LE]")
  {
    std::string testFile{ prefixPath("testfile026.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF16LE and leading spaces.", "[JSON][FromFile][UTF16LE][Whitespace]")
  {
    std::string testFile{ prefixPath("testfile027.json") };
    std::string expected{ R"(   [true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
}
// ======
// ToFile
// ======
TEST_CASE("Checks for toFile() api.", "[JSON][ToFile]")
{
  SECTION("Check that toFile() works with UTF8.", "[JSON][ToFile][UTF8]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF8BOM.", "[JSON][ToFile][UTF8BOM]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf8BOM);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BE.", "[JSON][ToFile][UTF16BE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16BE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BLE.", "[JSON][ToFile][UTF16LE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16LE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF32BE.", "[JSON][ToFile][UTF32BE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32BE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
  SECTION("Check that toFile() works with UTF32LE.", "[JSON][ToFile][UTF32LE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32LE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
}
