#include "JSON_Lib_Tests.hpp"

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

