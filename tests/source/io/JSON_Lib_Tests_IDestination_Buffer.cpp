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
    REQUIRE(buffer.size()==0);
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
}
