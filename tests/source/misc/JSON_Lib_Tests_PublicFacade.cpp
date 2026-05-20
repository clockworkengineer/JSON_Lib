#include "catch2/catch_all.hpp"
#include "JSON_Lib.hpp"
#include "JSON_IO.hpp"

using namespace JSON_Lib;

TEST_CASE("JSON_Lib.hpp public facade compiles with standard library-only includes", "[Dependency][PublicHeader]")
{
  JSON json;
  json.parse(BufferSource{R"({"status":"ok"})"});
  REQUIRE(json["status"].is<String>());

  BufferDestination destination;
  json.stringify(destination);
  REQUIRE(destination.toString() == R"({"status":"ok"})");
}

TEST_CASE("JSON_IO.hpp public I/O facade compiles independently", "[Dependency][PublicHeader]")
{
  BufferSource source{R"({"status":"ok"})"};
  BufferDestination destination;
  JSON json;
  json.parse(source);
  json.stringify(destination);
  REQUIRE(destination.toString() == R"({"status":"ok"})");
}
