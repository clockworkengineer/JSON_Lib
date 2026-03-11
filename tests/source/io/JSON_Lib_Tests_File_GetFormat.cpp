#include "JSON_Lib_Tests.hpp"

TEST_CASE("Checks for getFileFormat() api.", "[JSON][GetFileFormat]")
{
  SECTION("Check that getFileFormat() works with UTF8.", "[JSON][GetFileFormat][UTF8]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile021.json")) == JSON::Format::utf8);
  }
  SECTION("Check that getFileFormat() works with UTF8BOM.", "[JSON][GetFileFormat][UTF8BOM]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile022.json")) == JSON::Format::utf8BOM);
  }
  SECTION("Check that getFileFormat() works with UTF16BE.", "[JSON][GetFileFormat][UTF16BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile023.json")) == JSON::Format::utf16BE);
  }
  SECTION("Check that getFileFormat() works with UTF16LE.", "[JSON][GetFileFormat][UTF16LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile024.json")) == JSON::Format::utf16LE);
  }
  SECTION("Check that getFileFormat() works with UTF32BE.", "[JSON][GetFileFormat][UTF32BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile025.json")) == JSON::Format::utf32BE);
  }
  SECTION("Check that getFileFormat() works with UTF32LE.", "[JSON][GetFileFormat][UTF32LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath("testfile026.json")) == JSON::Format::utf32LE);
  }
  // Round-trip: write with toFile() then verify getFileFormat() returns the same format.
  SECTION("Check that getFileFormat() detects UTF8 written by toFile().", "[JSON][GetFileFormat][RoundTrip][UTF8]")
  {
    std::string testFile{ generateRandomFileName() };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf8);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf8);
    std::filesystem::remove(testFile);
  }
  SECTION(
    "Check that getFileFormat() detects UTF8BOM written by toFile().", "[JSON][GetFileFormat][RoundTrip][UTF8BOM]")
  {
    std::string testFile{ generateRandomFileName() };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf8BOM);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf8BOM);
    std::filesystem::remove(testFile);
  }
  SECTION(
    "Check that getFileFormat() detects UTF16BE written by toFile().", "[JSON][GetFileFormat][RoundTrip][UTF16BE]")
  {
    std::string testFile{ generateRandomFileName() };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf16BE);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf16BE);
    std::filesystem::remove(testFile);
  }
  SECTION(
    "Check that getFileFormat() detects UTF16LE written by toFile().", "[JSON][GetFileFormat][RoundTrip][UTF16LE]")
  {
    std::string testFile{ generateRandomFileName() };
    JSON::toFile(testFile, R"(["test"])", JSON::Format::utf16LE);
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf16LE);
    std::filesystem::remove(testFile);
  }
  // Edge cases: both an empty file and a non-existent file contain no recognisable
  // BOM bytes, so getFileFormat() falls back to UTF8.
  SECTION("Check that getFileFormat() falls back to UTF8 for an empty file.", "[JSON][GetFileFormat][Empty]")
  {
    std::string testFile{ generateRandomFileName() };
    {
      std::ofstream out{ testFile, std::ios::binary };
    }// create empty file
    REQUIRE(JSON::getFileFormat(testFile) == JSON::Format::utf8);
    std::filesystem::remove(testFile);
  }
  SECTION(
    "Check that getFileFormat() falls back to UTF8 for a non-existent file.", "[JSON][GetFileFormat][NonExistent]")
  {
    REQUIRE(JSON::getFileFormat(prefixTestDataPath(kNonExistantJSONFile)) == JSON::Format::utf8);
  }
}
