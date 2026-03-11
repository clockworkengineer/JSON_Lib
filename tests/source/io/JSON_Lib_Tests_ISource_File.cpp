#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check ISource (File) interface.", "[JSON][ISource][File]")
{
  SECTION("Create FileSource with testfile001.json.", "[JSON][ISource][File][Construct]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    REQUIRE_NOTHROW(FileSource(testFileName));
  }
  SECTION("Check that File position() works correctly.", "[JSON][ISource][File][Position]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ testFileName };
    while (source.more() && !source.match("time")) { source.next(); }
    REQUIRE(source.position() == 21);
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 43);
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create FileSource and check positioned on the correct first character.", "[JSON][ISource][File][Position]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    auto source{ FileSource(testFileName) };
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create FileSource and then check next positions to correct next character.", "[JSON][ISource][File][Next]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\n');
  }
  SECTION("Create FileSource move past last character, check it and the bytes moved.", "[JSON][ISource][File][More]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == std::filesystem::file_size(testFileName));// eof
    REQUIRE(source.current() == static_cast<char>(EOF));// eof
  }
  SECTION("Create FileSource move past last character, reset and then check back at the beginning.",
    "[JSON][ISource][File][Reset]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.position() == 0);//  Check at the beginning
  }
  SECTION("Check that FileSource is ignoring whitespace correctly.", "[JSON][ISource][File][Whitespace]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
    FileSource source{ testFileName };
    std::string strippedJSON;
    while (source.more()) {
      source.ignoreWS();
      strippedJSON += source.current();
      source.next();
    }
    REQUIRE(strippedJSON == R"({"Name":"Pete"})");
    REQUIRE(source.current() == static_cast<char>(EOF));
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Check that FileSource ignoreWS() at end of file does not throw but next() does.",
    "[JSON][ISource][File][Whitespace]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE_NOTHROW(source.ignoreWS());
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Check that FileSource finds a string at the current position and moves on past it in stream.",
    "[JSON][ISource][File][Match]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ testFileName };
    REQUIRE_FALSE(source.match("[trap"));// Not there
    REQUIRE_FALSE(!source.match("[true"));// Match
    REQUIRE(source.position() == 5);// new position
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create FileSource with non existent file.", "[JSON][ISource][File][Exception]")
  {
    const std::string nonExistentFileName{ prefixTestDataPath(kNonExistantJSONFile) };
    REQUIRE_THROWS_AS(FileSource(nonExistentFileName), ISource::Error);
    REQUIRE_THROWS_WITH(
      FileSource(nonExistentFileName), "ISource Error: File input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource and then try to read off he end.", "[JSON][ISource][File][Exception]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
  }
  SECTION("Create a FileSource that contains linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    BufferSource source{ R"([true\n,Out of time"\n,7.89043e+18\n,true\n])" };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    //  No translation of linefeed occurs
    REQUIRE(result == R"([true\n,Out of time"\n,7.89043e+18\n,true\n])");
  }
  SECTION("Create a FileSource that contains linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
    FileSource source{ testFileName };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    // No translation of just linefeed occurs when reading from source stream
    REQUIRE(result == "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create a FileSource that contains carriage return/linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    const std::string testFileName{ generateRandomFileName() };

    JSON::toFile(testFileName, "[true\r\n,\"Out of time\"\r\n,7.89043e+18\r\n,true\r\n]");
    FileSource source{ testFileName };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    // Carriage return/linefeed translated to linefeed when reading from source stream
    REQUIRE(result == "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create a FileSource that contains bare carriage return and verify it is passed through unchanged.",
    "[JSON][ISource][File][Linefeed]")
  {
    std::string result;
    const std::string testFileName{ generateRandomFileName() };
    // Write raw bytes with a bare \r (not followed by \n)
    {
      std::ofstream out{ testFileName, std::ios::binary };
      out << "a\rb";
    }
    FileSource source{ testFileName };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    // A bare \r (not followed by \n) is passed through as-is; only \r\n pairs are collapsed to \n
    REQUIRE(result == "a\rb");
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create FileSource, move to end, reset and verify current() returns first character.",
    "[JSON][ISource][File][Reset]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.current() == '{');
  }
  SECTION("Create FileSource, advance, reset and verify getPosition() returns {1,1}.", "[JSON][ISource][File][Reset]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    source.next();
    source.next();
    source.reset();
    REQUIRE(source.getPosition() == std::make_pair(1L, 1L));
  }
  SECTION(
    "Create FileSource and verify getPosition() tracks line and column correctly.", "[JSON][ISource][File][Position]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, "ab\ncd");
    FileSource source{ testFileName };
    REQUIRE(source.getPosition() == std::make_pair(1L, 1L));// on 'a'
    source.next();// advance to 'b'
    REQUIRE(source.getPosition() == std::make_pair(1L, 2L));// on 'b' line=1,col=2
    source.next();// advance to '\n' -> lineNo++, col=1
    REQUIRE(source.getPosition() == std::make_pair(2L, 1L));// on '\n' line=2,col=1
    source.next();// advance to 'c'
    REQUIRE(source.getPosition() == std::make_pair(2L, 2L));// on 'c' line=2,col=2
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create FileSource, call ignoreWS() on non-whitespace char - position should not change.",
    "[JSON][ISource][File][Whitespace]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    REQUIRE(source.position() == 0);
    source.ignoreWS();
    REQUIRE(source.position() == 0);
    REQUIRE(source.current() == '{');
  }
  SECTION("Create FileSource and check match() fails gracefully when target is longer than remaining file.",
    "[JSON][ISource][File][Match]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, "ab");
    FileSource source{ testFileName };
    REQUIRE_FALSE(source.match("abcde"));
    // position must be restored after failed match
    REQUIRE(source.position() == 0);
    REQUIRE(source.current() == 'a');
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION(
    "Create FileSource and check match() mid-stream advances to character after match.", "[JSON][ISource][File][Match]")
  {
    const std::string testFileName{ generateRandomFileName() };
    JSON::toFile(testFileName, R"([true,"key"])");
    FileSource source{ testFileName };
    source.next();// move past '['
    REQUIRE(source.match("true"));
    REQUIRE(source.position() == 5);
    REQUIRE(source.current() == ',');
    source.close();
    std::filesystem::remove(testFileName);
  }
  SECTION("Create FileSource and check getFileName() returns the correct path.", "[JSON][ISource][File][GetFileName]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    FileSource source{ testFileName };
    REQUIRE(source.getFileName() == testFileName);
  }
}
