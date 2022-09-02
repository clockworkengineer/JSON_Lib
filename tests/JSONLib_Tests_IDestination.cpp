//
// Unit Tests: JSONLib_Tests_IDestination
//
// Description: IDestination inteface unit tests for JSON class using
// the Catch2 test framework. At present the only supported destination
// streams are buffer and file.
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
// ======
// Buffer
// ======
TEST_CASE("IDestination (Buffer) interface.", "[JSON][IDestination][Buffer]")
{
  SECTION("Create BufferDestination.", "[JSON][IDestination][Buffer][Construct]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[JSON][IDestination][Buffer][Construct]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().empty());
  }
  SECTION("Create BufferDestination and add one character.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION("Create BufferDestination and add an integer string and check result.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
  SECTION("Create BufferDestination, add to it, clear buffer and then add to it again and check result.",
    "[JSON][IDestination][Buffer][Clear]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
    buffer.clear();
    REQUIRE(buffer.getBuffer().size() == 0);
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
  SECTION("Create BufferDestination and and add content with linefeeds.", "[JSON][IDestination][Buffer][Linefeed]")
  {
    BufferDestination buffer;
    buffer.add("65767\n");
    buffer.add("22222\n");
    buffer.add("33333\n");
    REQUIRE(buffer.getBuffer().size() == 18);
    REQUIRE(buffer.getBuffer() == "65767\n22222\n33333\n");
  }
}
// ====
// File
// ====
TEST_CASE("IDestination (File) interface.", "[JSON][IDestination][File]")
{
  const std::string testFileName{ prefixPath(kGeneratedJSONFile) };
  SECTION("Create FileDestination.", "[JSON][IDestination][File][Construct]")
  {
    std::filesystem::remove(testFileName);
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][IDestination][File][Construct]")
  {
    FileDestination file{ testFileName };
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][IDestination][File][Construct]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    REQUIRE_FALSE(!std::filesystem::exists(testFileName));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][IDestination][File][Add]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add('t');
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an integer string and check result.", "[JSON][IDestination][File][Add]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == "65767");
  }
  SECTION("Create FileDestination, add to it, clear file and then add to it again and check result.",
    "[JSON][IDestination][File][Clear]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    file.clear();
    REQUIRE(std::filesystem::file_size(filePath) == 0);
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
  }
  SECTION("Create FileDestination and and add content with linefeeds.", "[JSON][IDestination][Buffer][Linefeed]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767\n");
    file.add("22222\n");
    file.add("33333\n");
    REQUIRE(std::filesystem::file_size(filePath) == 21);
    REQUIRE(readFromFile(testFileName) == ("65767\r\n22222\r\n33333\r\n"));
  }
}
