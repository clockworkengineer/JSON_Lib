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
