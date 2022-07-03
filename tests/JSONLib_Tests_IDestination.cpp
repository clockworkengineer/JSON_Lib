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
// ======================
// JSON library namespace
// ======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
// =================
// BufferDestination
// =================
TEST_CASE("IDestination (Buffer) interface.", "[JSON][Stringify][IDestination]")
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
// ===============
// FileDestination
// ===============
TEST_CASE("IDestination (File) interface.", "[JSON][Parse][IDestination]")
{
  const std::string testFileName {prefixTestDataPath(kGeneratedJSONFile)};
  SECTION("Create FileDestination.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(testFileName);
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][Stringify][IDestination]")
  {
    FileDestination file{testFileName};
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{testFileName};
    std::filesystem::path filePath{testFileName};
    REQUIRE_FALSE(!std::filesystem::exists(testFileName));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{testFileName};
    std::filesystem::path filePath{testFileName};
    file.add('t');
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an stringified integer and check result.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{testFileName};
    std::filesystem::path filePath{testFileName};
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == "65767");
  }
}
