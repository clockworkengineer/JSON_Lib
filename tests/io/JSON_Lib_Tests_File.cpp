//
// Unit Tests: JSON_Lib_Tests_File
//
// Description:
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
TEST_CASE("Checks for getFileFormat() api.", "[JSON][getFileFormat]")
{
  SECTION("Check that getFileFormat() works with UTF8.", "[JSON][getFileFormat][UTF8]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile021.json")) == JSON::Format::utf8);
  }
  SECTION("Check that getFileFormat() works with UTF8BOM.", "[JSON][getFileFormat][UTF8BOM]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile022.json")) == JSON::Format::utf8BOM);
  }
  SECTION("Check that getFileFormat() works with UTF16BE.", "[JSON][getFileFormat][UTF16BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile023.json")) == JSON::Format::utf16BE);
  }
  SECTION("Check that getFileFormat() works with UTF16LE.", "[JSON][getFileFormat][UTF16LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile024.json")) == JSON::Format::utf16LE);
  }
  SECTION("Check that getFileFormat() works with UTF32BE.", "[JSON][getFileFormat][UTF32BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile025.json")) == JSON::Format::utf32BE);
  }
  SECTION("Check that getFileFormat() works with UTF32LE.", "[JSON][getFileFormat][UTF32LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile026.json")) == JSON::Format::utf32LE);
  }
}
TEST_CASE("Checks for fromFile() api.", "[JSON][fromFile]")
{
  SECTION("Check that fromFile() works with UTF8.", "[JSON][fromFile][UTF8]")
  {
    std::string testFile{ prefixPath("testfile021.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF8BOM.", "[JSON][fromFile][UTF8BOM]")
  {
    std::string testFile{ prefixPath("testfile022.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16BE.", "[JSON][fromFile][UTF16BE]")
  {
    std::string testFile{ prefixPath("testfile023.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF16LE.", "[JSON][fromFile][UTF16LE]")
  {
    std::string testFile{ prefixPath("testfile024.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that fromFile() works with UTF32BE.", "[JSON][fromFile][UTF32BE]")
  {
    std::string testFile{ prefixPath("testfile025.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF32LE.", "[JSON][fromFile][UTF32LE]")
  {
    std::string testFile{ prefixPath("testfile026.json") };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(
      JSON::fromFile(testFile), "JSON Error: Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  SECTION("Check that fromFile() works with UTF16LE and leading spaces.", "[JSON][fromFile][UTF16LE][Whitespace]")
  {
    std::string testFile{ prefixPath("testfile027.json") };
    std::string expected{ R"(   [true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
}
TEST_CASE("Checks for toFile() api.", "[JSON][toFile]")
{
  SECTION("Check that toFile() works with UTF8.", "[JSON][toFile][UTF8]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF8BOM.", "[JSON][toFile][UTF8BOM]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf8BOM);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BE.", "[JSON][toFile][UTF16BE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16BE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF16BLE.", "[JSON][toFile][UTF16LE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    JSON::toFile(testFile, expected, JSON::Format::utf16LE);
    REQUIRE(JSON::fromFile(testFile) == expected);
  }
  SECTION("Check that toFile() works with UTF32BE.", "[JSON][toFile][UTF32BE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32BE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
  SECTION("Check that toFile() works with UTF32LE.", "[JSON][toFile][UTF32LE]")
  {
    std::string testFile{ prefixPath(kGeneratedJSONFile) };
    std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_THROWS_WITH(JSON::toFile(testFile, expected, JSON::Format::utf32LE),
      "JSON Error: Unsupported JSON file format (Byte Order Mark) specified.");
  }
}
