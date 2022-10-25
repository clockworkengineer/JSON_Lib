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
TEST_CASE("Checks for getFileFormat() api", "[JSON][fromFile]")
{
  SECTION("Check that getFileFormat() works with UTF8.", "[JSON][getFileFormat][UTF8]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile020.json")) == JSON::Format::utf8);
  }
  SECTION("Check that getFileFormat() works with UTF8BOM.", "[JSON][getFileFormat][UTF8BOM]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile021.json")) == JSON::Format::utf8BOM);
  }
  SECTION("Check that getFileFormat() works with UTF16BE.", "[JSON][getFileFormat][UTF16BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile022.json")) == JSON::Format::utf16BE);
  }
  SECTION("Check that getFileFormat() works with UTF16LE.", "[JSON][getFileFormat][UTF16LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile023.json")) == JSON::Format::utf16LE);
  }
  SECTION("Check that getFileFormat() works with UTF32BE.", "[JSON][getFileFormat][UTF32BE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile024.json")) == JSON::Format::utf32BE);
  }
  SECTION("Check that getFileFormat() works with UTF32LE.", "[JSON][getFileFormat][UTF32LE]")
  {
    REQUIRE(JSON::getFileFormat(prefixPath("testfile025.json")) == JSON::Format::utf32LE);
  }
}
// TEST_CASE("Checks for fromFile() api", "[JSON][fromFile]")
// {
//   SECTION("Check that fromFile() works with UTF8.", "[JSON][fromFile][UTF8]")
//   {
//     std::string testFile{ prefixPath("testfile021.json") };
//     std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
//     REQUIRE(JSON::fromFile(testFile) == expected);
//   }
//   SECTION("Check that fromFile() works with UTF8BOM.", "[JSON][fromFile][UTF8BOM]")
//   {
//     std::string testFile{ prefixPath("testfile022.json") };
//     std::string expected{ R"([true  , "Out of time",  7.89043e+18, true])" };
//     REQUIRE(JSON::fromFile(testFile) == expected);
//   }
// }
