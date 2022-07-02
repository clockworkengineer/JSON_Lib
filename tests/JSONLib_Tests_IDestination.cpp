//
// Unit Tests: JSON
//
// Description: IDestination inteface unit tests for JSON class using 
// the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSONLib_Tests.hpp"
// =======================
// JSON class namespace
// =======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
TEST_CASE("IDestination (Buffer) interface.", "[JSON][Parse][IDestination]")
{
  SECTION("Create BufferDestination.", "[JSON][Stringify][IDestination]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().empty());
  }
  SECTION("Create BufferDestination and add one character.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION("Create BufferDestination and add an stringified integer and check result.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
}
TEST_CASE("IDestination (File) interface.", "[JSON][Parse][IDestination]")
{
  SECTION("Create FileDestination.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedJSONFile)));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][Stringify][IDestination]")
  {
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedJSONFile)));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    REQUIRE_FALSE(!std::filesystem::exists(prefixTestDataPath(kGeneratedJSONFile)));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    file.add('t');
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an stringified integer and check result.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) == "65767");
  }
}
