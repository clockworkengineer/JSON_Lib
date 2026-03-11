#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check IDestination (Buffer) interface.", "[JSON][IDestination][Buffer]")
{
  SECTION("Create BufferDestination.", "[JSON][IDestination][Buffer][Construct]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[JSON][IDestination][Buffer][Construct]")
  {
    BufferDestination buffer;
    REQUIRE(buffer.size() == 0);
  }
  SECTION("Create BufferDestination and add one character.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.size() == 1);
  }
  SECTION("Create BufferDestination and add an integer string and check result.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == ("65767"));
  }
  SECTION("Create BufferDestination, add to it, clear buffer and then add to it again and check result.",
    "[JSON][IDestination][Buffer][Clear]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == ("65767"));
    buffer.clear();
    REQUIRE(buffer.size() == 0);
    buffer.add("65767");
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == ("65767"));
  }
  SECTION("Create BufferDestination and and add content with linefeeds.", "[JSON][IDestination][Buffer][Linefeed]")
  {
    BufferDestination buffer;
    buffer.add("65767\n");
    buffer.add("22222\n");
    buffer.add("33333\n");
    REQUIRE(buffer.size() == 18);
    REQUIRE(buffer.toString() == "65767\n22222\n33333\n");
  }
  SECTION("Create BufferDestination, add single char and check toString().", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add('X');
    REQUIRE(buffer.toString() == "X");
  }
  SECTION("Create BufferDestination, add a std::string and check result.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    std::string str{ "hello" };
    buffer.add(str);
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == "hello");
  }
  SECTION("Create BufferDestination, add a std::string_view and check result.", "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    std::string_view sv{ "world" };
    buffer.add(sv);
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == "world");
  }
  SECTION("Create BufferDestination and check last() returns the most recently added character.",
    "[JSON][IDestination][Buffer][Last]")
  {
    BufferDestination buffer;
    buffer.add("abc");
    REQUIRE(buffer.last() == 'c');
    buffer.add('Z');
    REQUIRE(buffer.last() == 'Z');
  }
  SECTION("Create BufferDestination and verify size grows correctly across sequential adds.",
    "[JSON][IDestination][Buffer][Add]")
  {
    BufferDestination buffer;
    buffer.add("ab");
    REQUIRE(buffer.size() == 2);
    buffer.add("cd");
    REQUIRE(buffer.size() == 4);
    buffer.add('e');
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == "abcde");
  }
  SECTION("Create BufferDestination and use with JSON stringify.", "[JSON][IDestination][Buffer][Stringify]")
  {
    const JSON json(R"({"key":42})");
    BufferDestination buffer;
    json.stringify(buffer);
    REQUIRE_FALSE(buffer.toString().empty());
    const JSON roundtrip(buffer.toString());
    BufferDestination buffer2;
    roundtrip.stringify(buffer2);
    REQUIRE(buffer.toString() == buffer2.toString());
  }
}
