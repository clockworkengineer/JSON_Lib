//
// Unit Tests: JSON
//
// Description: ISource inteface unit tests for JSON class using the
// Catch2 test framework. At present the only supported source streams
// are buffer and file.
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
TEST_CASE("ISource (Buffer) interface (uses file testfile001.json).", "[JSON][ISource][Buffer]")
{
  const std::string testFileName{ prefixPath(kSingleJSONFile) };
  const std::string buffer{ readFromFile(testFileName) };
  SECTION("Create BufferSource.", "[JSON][ISource][Buffer]") { REQUIRE_NOTHROW(BufferSource(buffer)); }
  SECTION("Check that BufferSource position() works correctly.", "[JSON][ISource][Buffer]")
  {
    BufferSource source{ R"([true  , "Out of time",  7.89043e+18, true])" };
    while (source.more() && !source.match("time")) { source.next(); }
    REQUIRE(source.position() == 21);
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 43);
  }
  SECTION(
    "Create BufferSource and that it is positioned "
    "on the correct first character",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ BufferSource(buffer) };
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION(
    "Create BufferSource and then check next "
    "positions to correct next character",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ BufferSource(buffer) };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\r');
  }
  SECTION(
    "Create BufferSource move past last character, "
    "check it and the bytes moved.",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == std::filesystem::file_size(testFileName));// eof
    REQUIRE(source.current() == static_cast<char>(EOF));// eof
  }
  SECTION(
    "Create BufferSource, move past last character, "
    "reset and then check back at the beginning.",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.position() == 0);//  Check at the beginning
  }
  SECTION("Create BufferSource and check that it is ignoring whitespace correctly.", "[JSON][ISource][Buffer]")
  {
    BufferSource source{ "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}" };
    std::string strippedJSON;
    while (source.more()) {
      source.ignoreWS();
      strippedJSON += source.current();
      source.next();
    }
    REQUIRE(strippedJSON == R"({"Name":"Pete"})");
    REQUIRE(source.current() == static_cast<char>(EOF));
  }
  SECTION(
    "Check that BufferSource ignoreWS() at end of file does not throw "
    "but next() does.",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}" };
    while (source.more()) { source.next(); }
    REQUIRE_NOTHROW(source.ignoreWS());
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past and of buffer.");
  }
  SECTION(
    "Check that BufferSource finds a string at the current position and "
    "moves on past it in stream",
    "[JSON][ISource][Buffer]")
  {
    BufferSource source{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_FALSE(source.match("[trap"));// Not there
    REQUIRE_FALSE(!source.match("[true"));// Match
    REQUIRE(source.position() == 5);// new position
  }
  SECTION("Check that BufferSource backup works and doesn't go negative.", "[XML][Parse][BufferSource]")
  {
    BufferSource source{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_FALSE(!source.match(R"([true  , "Out )"));
    REQUIRE(source.current() == 'o');
    source.backup(4);
    REQUIRE(source.current() == 'O');
    source.backup(12);
    REQUIRE(source.current() == '[');
    while (source.more()) { source.next(); }
    REQUIRE(source.current() == static_cast<char>(EOF));
    source.backup(1);
    REQUIRE(source.current() == ']');
  }
  SECTION("Create BufferSource with empty buffer.", "[JSON][ISource][Buffer][Exception]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), ISource::Error);
    REQUIRE_THROWS_WITH(BufferSource(""), "ISource Error: Empty source buffer passed to be parsed.");
  }
  SECTION(
    "Create BufferSource and then try to read off "
    "the end",
    "[JSON][ISource][Buffer][Exception]")
  {
    BufferSource source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past and of buffer.");
  }
}
// ====
// File
// ====
TEST_CASE("ISource (File) interface.", "[JSON][ISource][File]")
{
  const std::string testFileName{ prefixPath(kSingleJSONFile) };
  SECTION("Create FileSource with testfile001.json.", "[JSON][ISource][File]")
  {
    REQUIRE_NOTHROW(FileSource(testFileName));
  }
  SECTION("Check that File position() works correctly.", "[JSON][ISource][File]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    writeToFile(kGeneratedJSONFile, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ kGeneratedJSONFile };
    while (source.more() && !source.match("time")) { source.next(); }
    REQUIRE(source.position() == 21);
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 43);
  }
  SECTION(
    "Create FileSource and check positioned on the "
    "correct first character",
    "[JSON][ISource][File]")
  {
    FileSource source{ FileSource(testFileName) };
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION(
    "Create FileSource and then check next "
    "positions to correct next character",
    "[JSON][ISource][File]")
  {
    FileSource source{ testFileName };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\n');
  }
  SECTION(
    "Create FileSource move past last character, "
    "check it and the bytes moved.",
    "[JSON][ISource][File]")
  {
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == std::filesystem::file_size(testFileName));// eof
    REQUIRE(source.current() == static_cast<char>(EOF));// eof
  }
  SECTION(
    "Create FileSource move past last character, "
    "reset and then check back at the beginning.",
    "[JSON][ISource][File]")
  {
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.position() == 0);//  Check at the beginning
  }
  SECTION("Check that FileSource is ignoring whitespace correctly.", "[JSON][ISource][File]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    writeToFile(kGeneratedJSONFile, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
    FileSource source{ kGeneratedJSONFile };
    std::string strippedJSON;
    while (source.more()) {
      source.ignoreWS();
      strippedJSON += source.current();
      source.next();
    }
    REQUIRE(strippedJSON == R"({"Name":"Pete"})");
    REQUIRE(source.current() == static_cast<char>(EOF));
  }
  SECTION(
    "Check that FileSource ignoreWS() at end of file does not throw "
    "but next() does.",
    "[JSON][ISource][File]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    writeToFile(kGeneratedJSONFile, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
    FileSource source{ kGeneratedJSONFile };
    while (source.more()) { source.next(); }
    REQUIRE_NOTHROW(source.ignoreWS());
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
  }
  SECTION(
    "Check that FileSource finds a string at the current position and "
    "moves on past it in stream",
    "[JSON][ISource][File]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    writeToFile(kGeneratedJSONFile, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ kGeneratedJSONFile };
    REQUIRE_FALSE(source.match("[trap"));// Not there
    REQUIRE_FALSE(!source.match("[true"));// Match
    REQUIRE(source.position() == 5);// new position
  }
  SECTION("Check that FileSource backup works and doesn't go negative.", "[XML][Parse][File]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    writeToFile(kGeneratedJSONFile, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ kGeneratedJSONFile };
    REQUIRE_FALSE(!source.match(R"([true  , "Out )"));
    REQUIRE(source.current() == 'o');
    source.backup(4);
    REQUIRE(source.current() == 'O');
    source.backup(12);
    REQUIRE(source.current() == '[');
    while (source.more()) { source.next(); }
    REQUIRE(source.current() == static_cast<char>(EOF));
    source.backup(1);
    REQUIRE(source.current() == ']');
  }
  SECTION("Create FileSource with non existant file.", "[JSON][ISource][File][Exception]")
  {
    const std::string nonExistantFileName{ prefixPath(kNonExistantJSONFile) };
    REQUIRE_THROWS_AS(FileSource(nonExistantFileName), ISource::Error);
    REQUIRE_THROWS_WITH(
      FileSource(nonExistantFileName), "ISource Error: File input stream failed to open or does not exist.");
  }
  SECTION(
    "Create FileSource and then try to read off "
    "the end",
    "[JSON][ISource][File][Exception]")
  {
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
  }
}
