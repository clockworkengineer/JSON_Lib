#include "catch2/catch_all.hpp"
#include "JSON_Lib.hpp"

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
