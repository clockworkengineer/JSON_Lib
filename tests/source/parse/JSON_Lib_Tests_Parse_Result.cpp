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

TEST_CASE("JSON::parseResult rejects invalid UTF-8 sequences", "[JSON][Parse][Result][UTF8]")
{
  JSON json;
  std::string invalidUtf8 = "[\"";
  invalidUtf8.push_back(static_cast<char>(0xC0));
  invalidUtf8 += "\"]";

  auto result = json.parseResult(BufferSource{invalidUtf8});
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::UnsupportedEncoding);
  REQUIRE(result.message.find("Invalid UTF-8 sequence in string.") != std::string::npos);
}

TEST_CASE("JSON::parseResult rejects parser depth exceedance", "[JSON][Parse][Result][Depth]")
{
  JSON json;
  const std::string deep = std::string(129, '[') + std::string(129, ']');
  auto result = json.parseResult(BufferSource{deep});
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::SyntaxError);
  REQUIRE(result.message.find("Maximum parser depth exceeded.") != std::string::npos);
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

#if JSON_LIB_NO_HEAP
TEST_CASE("EmbeddedJSON reports no-heap build policy", "[JSON][Build][NoHeap]")
{
  REQUIRE(JSON_LIB_NO_DYNAMIC_MEMORY);
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild());
}
#endif

