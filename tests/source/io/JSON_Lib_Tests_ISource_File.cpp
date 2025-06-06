#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check ISource (File) interface.", "[JSON][ISource][File]")
{
  const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
  SECTION("Create FileSource with testfile001.json.", "[JSON][ISource][File][Construct]")
  {
    REQUIRE_NOTHROW(FileSource(testFileName));
  }
  SECTION("Check that File position() works correctly.", "[JSON][ISource][File][Position]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ kGeneratedJSONFile };
    while (source.more() && !source.match("time")) { source.next(); }
    REQUIRE(source.position() == 21);
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 43);
  }
  SECTION("Create FileSource and check positioned on the correct first character.", "[JSON][ISource][File][Position]")
  {
    auto source{ FileSource(testFileName) };
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create FileSource and then check next positions to correct next character.", "[JSON][ISource][File][Next]")
  {
    FileSource source{ testFileName };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\n');
  }
  SECTION("Create FileSource move past last character, check it and the bytes moved.", "[JSON][ISource][File][More]")
  {
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == std::filesystem::file_size(testFileName));// eof
    REQUIRE(source.current() == static_cast<char>(EOF));// eof
  }
  SECTION("Create FileSource move past last character, reset and then check back at the beginning.",
    "[JSON][ISource][File][Reset]")
  {
    FileSource source{ testFileName };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.position() == 0);//  Check at the beginning
  }
  SECTION("Check that FileSource is ignoring whitespace correctly.", "[JSON][ISource][File][Whitespace]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
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
  SECTION("Check that FileSource ignoreWS() at end of file does not throw but next() does.",
    "[JSON][ISource][File][Whitespace]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}");
    FileSource source{ kGeneratedJSONFile };
    while (source.more()) { source.next(); }
    REQUIRE_NOTHROW(source.ignoreWS());
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
  }
  SECTION("Check that FileSource finds a string at the current position and moves on past it in stream.",
    "[JSON][ISource][File][Match]")
  {
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, R"([true  , "Out of time",  7.89043e+18, true])");
    FileSource source{ kGeneratedJSONFile };
    REQUIRE_FALSE(source.match("[trap"));// Not there
    REQUIRE_FALSE(!source.match("[true"));// Match
    REQUIRE(source.position() == 5);// new position
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
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
    FileSource source{ kGeneratedJSONFile };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    // No translation of just linefeed occurs when reading from source stream
    REQUIRE(result == "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
  }
  SECTION("Create a FileSource that contains carriage return/linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    std::filesystem::remove(kGeneratedJSONFile);
    JSON::toFile(kGeneratedJSONFile, "[true\r\n,\"Out of time\"\r\n,7.89043e+18\r\n,true\r\n]");
    FileSource source{ kGeneratedJSONFile };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    // Carriage return/linefeed translated to linefeed when reading from source stream
    REQUIRE(result == "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
  }
}
