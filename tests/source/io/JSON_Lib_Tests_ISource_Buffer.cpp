#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check ISource (Buffer) interface.", "[JSON][ISource][Buffer]")
{
  const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };// Uses file testfile001.json
  const std::string buffer{ JSON::fromFile(testFileName) };
  SECTION("Create BufferSource.", "[JSON][ISource][Buffer][Construct]") { REQUIRE_NOTHROW(BufferSource(buffer)); }
  SECTION("Check that BufferSource position() works correctly.", "[JSON][ISource][Buffer][Position]")
  {
    BufferSource source{ R"([true  , "Out of time",  7.89043e+18, true])" };
    while (source.more() && !source.match("time")) { source.next(); }
    REQUIRE(source.position() == 21);
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 43);
  }
  SECTION("Create BufferSource and that it is positioned on the correct first character.",
    "[JSON][ISource][Buffer][Position]")
  {
    auto source{ BufferSource(buffer) };
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION(
    "Create BufferSource and then check next positions to correct next character", "[JSON][ISource][Buffer][Next]")
  {
    auto source{ BufferSource(buffer) };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\n');
  }
  SECTION(
    "Create BufferSource move past last character, check it and the bytes moved.", "[JSON][ISource][Buffer][More]")
  {
    auto source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    REQUIRE(source.position() == 787);// eof
    REQUIRE(source.current() == static_cast<char>(EOF));// eof
  }
  SECTION("Create BufferSource, move past last character, reset and then check back at the beginning.",
    "[JSON][ISource][Buffer][Reset]")
  {
    auto source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.position() == 0);//  Check at the beginning
  }
  SECTION(
    "Create BufferSource and check that it is ignoring whitespace correctly.", "[JSON][ISource][Buffer][Whitespace]")
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
  SECTION("Check that BufferSource ignoreWS() at end of file does not throw but next() does.",
    "[JSON][ISource][Buffer][Whitespace]")
  {
    BufferSource source{ "{    \"Name\" \t\t\t\r\r\r\r\r\r:\r\n       \"Pete\"   \r\r\r\r}" };
    while (source.more()) { source.next(); }
    REQUIRE_NOTHROW(source.ignoreWS());
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past and of buffer.");
  }
  SECTION("Check that BufferSource finds a string at the current position and moves on past it in stream.",
    "[JSON][ISource][Buffer][Match]")
  {
    BufferSource source{ R"([true  , "Out of time",  7.89043e+18, true])" };
    REQUIRE_FALSE(source.match("[trap"));// Not there
    REQUIRE_FALSE(!source.match("[true"));// Match
    REQUIRE(source.position() == 5);// new position
  }
  SECTION("Create BufferSource with empty buffer.", "[JSON][ISource][Buffer][Exception]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), ISource::Error);
    REQUIRE_THROWS_WITH(BufferSource(""), "ISource Error: Empty source buffer passed to be parsed.");
  }
  SECTION("Create BufferSource from string_view without owning a std::string.", "[JSON][ISource][Buffer][View]")
  {
    const std::string_view text{R"({"key":true})"};
    BufferSource source{text};
    REQUIRE(source.current() == '{');
    REQUIRE(source.position() == 0);
  }
  SECTION("Create BufferSource and then try to read off the end.", "[JSON][ISource][Buffer][Exception]")
  {
    auto source{ BufferSource(buffer) };
    while (source.more()) { source.next(); }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past and of buffer.");
  }
  SECTION("Create a BufferSource that contains linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    BufferSource source{ "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]" };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    //  No translation of linefeed occurs
    REQUIRE(result == "[true\n,\"Out of time\"\n,7.89043e+18\n,true\n]");
  }
  SECTION("Create a BufferSource that contains carriage return/linefeed and read from it into a string and compare.",
    "[JSON][ISource][Buffer][Linefeed]")
  {
    std::string result;
    BufferSource source{ "[true\r\n,\"Out of time\"\r\n,7.89043e+18\r\n,true\r\n]" };
    while (source.more()) {
      result.push_back(source.current());
      source.next();
    }
    //  No translation of carriage return/linefeed occurs
    REQUIRE(result == "[true\r\n,\"Out of time\"\r\n,7.89043e+18\r\n,true\r\n]");
  }
  SECTION("Create BufferSource, move to end, reset and verify current() returns first character.",
    "[JSON][ISource][Buffer][Reset]")
  {
    BufferSource source{ R"({"key":1})" };
    while (source.more()) { source.next(); }
    source.reset();
    REQUIRE(source.current() == '{');
  }
  SECTION(
    "Create BufferSource, advance, reset and verify getPosition() returns {1,1}.", "[JSON][ISource][Buffer][Reset]")
  {
    BufferSource source{ R"({"key":1})" };
    source.next();
    source.next();
    source.reset();
    REQUIRE(source.getPosition() == std::make_pair(1L, 1L));
  }
  SECTION("Create BufferSource and verify getPosition() tracks line and column correctly.",
    "[JSON][ISource][Buffer][Position]")
  {
    // next() increments column then, if the NEW current char is '\n', bumps lineNo and resets column to 1.
    // So getPosition() reflects where the source *is now*, not where it just came from.
    BufferSource source{ "ab\ncd" };
    REQUIRE(source.getPosition() == std::make_pair(1L, 1L));// on 'a'  line=1, col=1
    source.next();// advance to 'b', col becomes 2
    REQUIRE(source.getPosition() == std::make_pair(1L, 2L));// on 'b'  line=1, col=2
    source.next();// advance to '\n' -> lineNo++, col=1
    REQUIRE(source.getPosition() == std::make_pair(2L, 1L));// on '\n' line=2, col=1
    source.next();// advance to 'c', col becomes 2
    REQUIRE(source.getPosition() == std::make_pair(2L, 2L));// on 'c'  line=2, col=2
  }
  SECTION("Create BufferSource, call ignoreWS() when already on non-whitespace - position should not change.",
    "[JSON][ISource][Buffer][Whitespace]")
  {
    BufferSource source{ R"({"key":1})" };
    REQUIRE(source.position() == 0);
    source.ignoreWS();
    REQUIRE(source.position() == 0);
    REQUIRE(source.current() == '{');
  }
  SECTION("Create BufferSource and check match() fails gracefully when target is longer than remaining buffer.",
    "[JSON][ISource][Buffer][Match]")
  {
    BufferSource source{ "ab" };
    REQUIRE_FALSE(source.match("abcde"));
    // After failed match, position must be restored
    REQUIRE(source.position() == 0);
    REQUIRE(source.current() == 'a');
  }
  SECTION("Create BufferSource and check match() in middle of stream advances to character after match.",
    "[JSON][ISource][Buffer][Match]")
  {
    BufferSource source{ R"([true,"key"])" };
    source.next();// move past '['
    REQUIRE(source.match("true"));
    REQUIRE(source.position() == 5);
    REQUIRE(source.current() == ',');
  }
  SECTION("Create BufferSource with a single character.", "[JSON][ISource][Buffer][Construct]")
  {
    BufferSource source{ "x" };
    REQUIRE(source.more());
    REQUIRE(source.current() == 'x');
    source.next();
    REQUIRE_FALSE(source.more());
    REQUIRE(source.current() == static_cast<char>(EOF));
  }
}
