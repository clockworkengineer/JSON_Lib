//
// Unit Tests: JSON
//
// Description: ISource inteface unit tests for JSON class using the
// Catch2 test framework.
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
// ============
// BufferSource
// ============
TEST_CASE("ISource (Buffer) interface. Contains file testfile001.json.",
          "[JSON][ISource][Buffer]") {
  const std::string testFileName{prefixTestDataPath(kSingleJSONFile)};
  const std::string buffer{readFromFile(testFileName)};
  SECTION("Create BufferSource.", "[JSON][ISource][Buffer]") {
    REQUIRE_NOTHROW(BufferSource(buffer));
  }
  SECTION("Create BufferSource with testfile001.json and that it is positioned "
          "on the correct first character",
          "[JSON][ISource][Buffer]") {
    BufferSource source{BufferSource(buffer)};
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create BufferSource with testfile001.json and then check next "
          "positions to correct next character",
          "[JSON][ISource][Buffer]") {
    BufferSource source{BufferSource(buffer)};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\r');
  }
  SECTION("Create BufferSource with testfile001.json move past last character, "
          "check it and the bytes moved.",
          "[JSON][ISource][Buffer]") {
    BufferSource source{BufferSource(buffer)};
    while (source.more()) {
      source.next();
    }
    REQUIRE(source.position() == 605);                   // eof
    REQUIRE(source.current() == static_cast<char>(EOF)); // eof
  }
  SECTION("Create BufferSource with empty buffer.",
          "[JSON][ISource][Buffer][Exception]") {
    REQUIRE_THROWS_AS(BufferSource(""), ISource::Error);
    REQUIRE_THROWS_WITH(
        BufferSource(""),
        "ISource Error: Empty source buffer passed to be parsed.");
  }
  SECTION("Create BufferSource with testfile001.json and then try to read off "
          "the end",
          "[JSON][ISource][Buffer][Exception]") {
    BufferSource source{BufferSource(buffer)};
    while (source.more()) {
      source.next();
    }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(),
                        "ISource Error: Tried to read past and of buffer.");
  }
}
// ==========
// FileSource
// ==========
TEST_CASE("ISource (File) interface.", "[JSON][ISource][File]") {
  const std::string testFileName{prefixTestDataPath(kSingleJSONFile)};
  SECTION("Create FileSource with testfile001.json.",
          "[JSON][ISource][File]") {
    REQUIRE_NOTHROW(FileSource(testFileName));
  }
  SECTION("Create FileSource with testfile001.json. and positioned on the "
          "correct first character",
          "[JSON][ISource][File]") {
    FileSource source{FileSource(testFileName)};
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create FileSource with testfile001.json and then check next "
          "positions to correct next character",
          "[JSON][ISource][File]") {
    FileSource source = FileSource(testFileName);
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\r');
  }
  SECTION("Create FileSource with non existant file.",
          "[JSON][ISource][File][Exception]") {
    const std::string nonExistantFileName{
        prefixTestDataPath(kNonExistantJSONFile)};
    REQUIRE_THROWS_AS(FileSource(nonExistantFileName), ISource::Error);
    REQUIRE_THROWS_WITH(
        FileSource(nonExistantFileName),
        "ISource Error: File input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource with testfile001.json and then try to read off "
          "the end",
          "[JSON][ISource][File][Exception]") {
    FileSource source = FileSource(testFileName);
    while (source.more()) {
      source.next();
    }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(),
                        "ISource Error: Tried to read past end of file.");
  }
}
