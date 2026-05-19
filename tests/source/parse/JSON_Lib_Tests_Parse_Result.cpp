#include "JSON_Lib_Tests.hpp"

TEST_CASE("JSON::parseResult returns Ok and populates the root tree", "[JSON][Parse][Result]")
{
  JSON json;
  auto result = json.parseResult(BufferSource{ R"({"name":"Alice","active":true})" });

  REQUIRE(result.ok());
  REQUIRE(result.value);
  REQUIRE(json.root().is<Object>());
  REQUIRE(json["name"].is<String>());
  REQUIRE(json["active"].is<Boolean>());
}

TEST_CASE("JSON::stringifyResult returns Ok for a populated JSON tree", "[JSON][Stringify][Result]")
{
  JSON json;
  json.parse(BufferSource{ R"({"tag":"result","count":2})" });
  BufferDestination destination;

  auto result = json.stringifyResult(destination);
  REQUIRE(result.ok());
  REQUIRE(destination.toString() == R"({"tag":"result","count":2})");
}

TEST_CASE("JSON::stringifyResult returns InvalidInput when JSON is empty", "[JSON][Stringify][Result]")
{
  JSON json;
  BufferDestination destination;

  auto result = json.stringifyResult(destination);
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::InvalidInput);
}

#if JSON_LIB_NO_EXCEPTIONS
TEST_CASE("JSON::parseResult works under no-exceptions builds", "[JSON][Parse][Result][NoExceptions]")
{
  JSON json;
  auto result = json.parseResult(BufferSource{ R"({"noExceptions":true})" });

  REQUIRE(result.ok());
  REQUIRE(json["noExceptions"].is<Boolean>());
}
#endif

#if JSON_LIB_NO_STDIO
TEST_CASE("EmbeddedJSON reports no-stdio build policy", "[JSON][Build][NoStdio]")
{
  REQUIRE(EmbeddedJSON::isNoStdIoBuild());
}
#endif

#if JSON_LIB_NO_DYNAMIC_MEMORY
TEST_CASE("EmbeddedJSON reports no-dynamic-memory build policy", "[JSON][Build][NoDynamicMemory]")
{
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild());
}
#endif
