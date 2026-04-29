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
