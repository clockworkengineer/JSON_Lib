#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check IDestination (FixedBuffer) interface.", "[JSON][IDestination][FixedBuffer]")
{
  FixedBufferDestination<16> buffer;
  buffer.add("hello");
  buffer.add(',');
  buffer.add("world");
  REQUIRE(buffer.size() == 11);
  REQUIRE(buffer.toString() == "hello,world");
  REQUIRE(buffer.last() == 'd');
}

TEST_CASE("Check FixedBufferDestination overflow error.", "[JSON][IDestination][FixedBuffer][Overflow]")
{
  FixedBufferDestination<4> buffer;
  REQUIRE_THROWS_AS(buffer.add("abcde"), IDestination::Error);
}
