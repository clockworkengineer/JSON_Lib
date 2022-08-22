//
// Unit Tests: JSON
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
  SECTION("Create BufferDestination.", "[JSON][IDestination][Buffer]") { REQUIRE_NOTHROW(BufferDestination()); }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().empty());
  }
  SECTION("Create BufferDestination and add one character.", "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION(
    "Create BufferDestination and add an stringified integer and check "
    "result.",
    "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
  SECTION(
    "Create BufferDestination, add to it, clear buffer and then add to it again and check."
    "result.",
    "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
    buffer.clear();
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
  SECTION("Create BufferDestination, add number to it, backup 2 and then check result.", "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
    buffer.backup();
    buffer.backup();
    REQUIRE(buffer.getBuffer().size() == 3);
    REQUIRE(buffer.getBuffer() == ("657"));
  }

  SECTION(
    "Create BufferDestination, add to it, backup 5 and then check its empty."
    "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
    buffer.backup();
    buffer.backup();
    buffer.backup();
    buffer.backup();
    buffer.backup();
    // At position 0 and destination empty
    REQUIRE(buffer.getBuffer().size() == 0);
    REQUIRE(buffer.getBuffer() == (""));
  }
  SECTION("Create BufferDestination, add to it and try to backup past beginning  and write out new value and check.",
    "[JSON][IDestination][Buffer]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
    buffer.backup();
    buffer.backup();
    buffer.backup();
    buffer.backup();
    buffer.backup();
    // At position 0 and destination empty
    REQUIRE(buffer.getBuffer().size() == 0);
    REQUIRE(buffer.getBuffer() == (""));
    // Try to backup past beginning of destination (does nothing)
    buffer.backup();
    // Stil at position 0 and destination empty
    REQUIRE(buffer.getBuffer().size() == 0);
    REQUIRE(buffer.getBuffer() == (""));
    // Write new contents
    buffer.add("999999");
    REQUIRE(buffer.getBuffer().size() == 6);
    REQUIRE(buffer.getBuffer() == ("999999"));
  }
}
// ====
// File
// ====
TEST_CASE("IDestination (File) interface.", "[JSON][IDestination][File]")
{
  const std::string testFileName{ prefixPath(kGeneratedJSONFile) };
  SECTION("Create FileDestination.", "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][IDestination][File]")
  {
    FileDestination file{ testFileName };
    REQUIRE_NOTHROW(FileDestination(testFileName));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    REQUIRE_FALSE(!std::filesystem::exists(testFileName));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add('t');
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an stringified integer and check result.", "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == "65767");
  }
  SECTION(
    "Create FileDestination, add to it, clear buffer and then add to it again and check."
    "result.",
    "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    file.clear();
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
  }
  SECTION(
    "Create FileDestination, add number to it, backup 2  add and then check result.", "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    file.backup();
    file.backup();
    file.add("68");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65768"));
  }

  SECTION(
    "Create FileDestination, add to it, backup 5 and write a new value and check."
    "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    file.backup();
    file.backup();
    file.backup();
    file.backup();
    file.backup();
    // Now at position 0 in file
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    // Overwrite existing contents
    file.add("999976");
    REQUIRE(std::filesystem::file_size(filePath) == 6);
    REQUIRE(readFromFile(testFileName) == ("999976"));
  }
  SECTION(
    "Create BufferDestination, add to it and try to backup past beginning and write a new value and check."
    "[JSON][IDestination][File]")
  {
    std::filesystem::remove(testFileName);
    FileDestination file{ testFileName };
    std::filesystem::path filePath{ testFileName };
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    file.backup();
    file.backup();
    file.backup();
    file.backup();
    file.backup();
    // Now at position 0 in file
    // Old content still there
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    // Try to back up past beginning of destination (does nothing)
    file.backup();
    // Still at position 0 in file
    // Old content still there
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readFromFile(testFileName) == ("65767"));
    // Overwrite existing contents
    file.add("999999");
    REQUIRE(std::filesystem::file_size(filePath) == 6);
    REQUIRE(readFromFile(testFileName) == ("999999"));
  }
}
