#include "JSON_Lib_Tests.hpp"
#include "XML_Stringify.hpp"
#include "YAML_Stringify.hpp"
#include "Bencode_Stringify.hpp"

TEST_CASE("Check JSON stringify generated exceptions.", "[JSON][Stringify][Exception]")
{
  SECTION("Stringify has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    const JSON json;
    REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
  }
  SECTION("Print has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    const JSON json;
    REQUIRE_THROWS_AS(json.print(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.print(BufferDestination{}), "JSON Error: No JSON to print.");
  }
  SECTION("XML stringify has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    const JSON json(makeStringify<XML_Stringify>());
    REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
  }
  SECTION("YAML stringify has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    const JSON json(makeStringify<YAML_Stringify>());
    REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
  }
  SECTION("Bencode stringify has no JSON to process.", "[JSON][Stringify][Exception]")
  {
    const JSON json(makeStringify<Bencode_Stringify>());
    REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
  }
  SECTION("Stringify does not throw after a successful parse.", "[JSON][Stringify][Exception]")
  {
    const JSON json;
    json.parse(BufferSource{ R"({"key":42})" });
    REQUIRE_NOTHROW(json.stringify(BufferDestination{}));
  }
  SECTION("Stringify can be called multiple times without throwing.", "[JSON][Stringify][Exception]")
  {
    const JSON json;
    json.parse(BufferSource{ R"([1,2,3])" });
    BufferDestination dest1;
    BufferDestination dest2;
    REQUIRE_NOTHROW(json.stringify(dest1));
    REQUIRE_NOTHROW(json.stringify(dest2));
    REQUIRE(dest1.toString() == dest2.toString());
  }
}