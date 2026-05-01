#include "JSON_Lib_Tests.hpp"

TEST_CASE("EmbeddedJSON API entrypoint build checks", "[JSON][Embedded][API]")
{
  REQUIRE(EmbeddedJSON::isEmbeddedBuild() == JSON_LIB_EMBEDDED);
  REQUIRE(EmbeddedJSON::isExceptionFreeBuild() == JSON_LIB_NO_EXCEPTIONS);
  REQUIRE(EmbeddedJSON::isNoStdIoBuild() == JSON_LIB_NO_STDIO);
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild() == JSON_LIB_NO_DYNAMIC_MEMORY);
  REQUIRE(EmbeddedJSON::Limits::maxParserDepth() >= 1);
  REQUIRE(EmbeddedJSON::Limits::maxStringLength() >= 1);
}

TEST_CASE("EmbeddedJSON parse and stringify buffer", "[JSON][Embedded][Buffer]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"x":true})" });
  BufferDestination destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"x":true})");
}

TEST_CASE("EmbeddedJSON fixed buffer stringify round-trip", "[JSON][Embedded][Buffer][FixedBuffer]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"embedded":true,"value":"small"})" });
  FixedBufferDestination<64> destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"embedded":true,"value":"small"})");
  REQUIRE(destination.size() <= 64);
}

TEST_CASE("EmbeddedJSON fixed buffer stringify overflow", "[JSON][Embedded][Buffer][FixedBuffer][Overflow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"overflow":"this will not fit"})" });
  FixedBufferDestination<8> destination;
  REQUIRE_THROWS_AS(embedded.stringify(destination), IDestination::Error);
}

TEST_CASE("EmbeddedJSON validation of embedded build policies", "[JSON][Embedded][Policy]")
{
  REQUIRE(EmbeddedJSON::isEmbeddedBuild() == JSON_LIB_EMBEDDED);
  REQUIRE(EmbeddedJSON::isExceptionFreeBuild() == JSON_LIB_NO_EXCEPTIONS);
  REQUIRE(EmbeddedJSON::isNoStdIoBuild() == JSON_LIB_NO_STDIO);
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild() == JSON_LIB_NO_DYNAMIC_MEMORY);
  REQUIRE(EmbeddedJSON::Limits::maxParserDepth() >= 1);
  REQUIRE(EmbeddedJSON::Limits::maxStringLength() >= 1);
}

TEST_CASE("FixedBufferSource parse simple JSON", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"({"key":42})";
  EmbeddedJSON embedded;
  embedded.parse(FixedBufferSource{ json, sizeof(json) - 1 });
  BufferDestination destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"key":42})");
}

TEST_CASE("FixedBufferSource parse and round-trip to FixedBufferDestination", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"({"a":1,"b":true,"c":"hello"})";
  EmbeddedJSON embedded;
  embedded.parse(FixedBufferSource{ json, sizeof(json) - 1 });
  FixedBufferDestination<64> destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == std::string(json));
}

TEST_CASE("FixedBufferSource reset re-parses correctly", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"([1,2,3])";
  FixedBufferSource source{ json, sizeof(json) - 1 };
  EmbeddedJSON first;
  first.parse(source);
  BufferDestination d1;
  first.stringify(d1);
  // reset and re-parse
  source.reset();
  EmbeddedJSON second;
  second.parse(source);
  BufferDestination d2;
  second.stringify(d2);
  REQUIRE(d1.toString() == d2.toString());
}

TEST_CASE("FixedBufferSource rejects null pointer", "[JSON][Embedded][FixedBufferSource]")
{
  REQUIRE_THROWS_AS((FixedBufferSource{ nullptr, 10 }), ISource::Error);
}

TEST_CASE("FixedBufferSource rejects zero length", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = "{}";
  REQUIRE_THROWS_AS((FixedBufferSource{ json, 0 }), ISource::Error);
}

#if JSON_LIB_NO_EXCEPTIONS
TEST_CASE("FixedBufferDestination overflow sets flag when NO_EXCEPTIONS", "[JSON][Embedded][FixedBufferDestination][NoExceptions]")
{
  FixedBufferDestination<4> dest;
  dest.add("12345"); // 5 bytes > capacity 4
  REQUIRE(dest.overflowed());
  dest.clearOverflow();
  REQUIRE_FALSE(dest.overflowed());
}
#else
TEST_CASE("FixedBufferDestination overflow throws in normal build", "[JSON][Embedded][FixedBufferDestination][Overflow]")
{
  FixedBufferDestination<4> dest;
  REQUIRE_THROWS_AS(dest.add("12345"), IDestination::Error);
}
#endif

TEST_CASE("EmbeddedJSON parseNoThrow returns Ok on valid JSON", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  const auto result = embedded.parseNoThrow(BufferSource{ R"({"x":1})" });
  REQUIRE(result.ok());
}

TEST_CASE("EmbeddedJSON stringifyNoThrow returns Ok after parse", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"x":1})" });
  BufferDestination destination;
  const auto result = embedded.stringifyNoThrow(destination);
  REQUIRE(result.ok());
  REQUIRE(destination.toString() == R"({"x":1})");
}

TEST_CASE("EmbeddedJSON stringifyNoThrow returns InvalidInput on empty JSON", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  BufferDestination destination;
  const auto result = embedded.stringifyNoThrow(destination);
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::InvalidInput);
}

TEST_CASE("EmbeddedJSON printNoThrow returns Ok after parse", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"([1,2,3])" });
  BufferDestination destination;
  const auto result = embedded.printNoThrow(destination);
  REQUIRE(result.ok());
}

TEST_CASE("EmbeddedJSON parseNoThrow rvalue overload", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  const auto result = embedded.parseNoThrow(BufferSource{ R"(true)" });
  REQUIRE(result.ok());
}
