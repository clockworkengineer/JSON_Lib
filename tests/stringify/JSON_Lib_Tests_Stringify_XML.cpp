//
// Unit Tests: JSON_Lib_Tests_XML_Stringify
//
// Description: XML stringification of simple/collection types unit
// tests for JSON class using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"
#include "XML_Stringify.hpp"

using namespace JSON_Lib;

// =========================================================
// Stringification of simple types to XML and validation
// =========================================================
TEST_CASE("Check JSON stringification to XML of simple types.", "[JSON][Stringify][Simple][XML]")
{
  const JSON json(std::make_unique<XML_Stringify>().release());
  SECTION("Stringify a string (Test string) to XML and check its value.", "[JSON][Stringify][String][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("Test string.")" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"(<?xml version="1.0" encoding="UTF-8"?><root>Test string.</root>)");
  }
  //   SECTION("Stringify a boolean (true) to XML and check its value.", "[JSON][Stringify][Boolean][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ "true" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "4:True");
  //   }
  //   SECTION("Stringify a boolean (false) to XML and check its value.", "[JSON][Stringify][Boolean][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ "false" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "5:False");
  //   }
  //   SECTION("Stringify a integer (98345) to XML and check its value.", "[JSON][Stringify][Integer][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ "98345" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "i98345e");
  //   }
  //   SECTION("Stringify a floating point (55.6667) to XML and check its value.", "[JSON][Stringify][Float][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ "55.6667" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "i55e");
  //   }
  //   SECTION("Stringify a null to XML and check its value.", "[JSON][Stringify][Simple][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ { "null" } });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "4:null");
  //   }
  //   SECTION("Stringify an array to XML and check its value.", "[JSON][Stringify][Array][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ { "[1,444,555,666,67.99]" } });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "li1ei444ei555ei666ei67ee");
  //   }
  //   SECTION("Stringify an object to XML and check its value.", "[JSON][Stringify][Object][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ { R"({"Age":77,"Name":"Rob"})" } });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "d3:Agei77e4:Name3:Robe");
  //   }
  //   SECTION(R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to XML and check its value.)",
  //     "[JSON][Stringify][Array][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ R"({"City":"London","Population":[1,2,3,4,5]})" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "d4:City6:London10:Populationli1ei2ei3ei4ei5eee");
  //   }
  //   SECTION(
  //     R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer and check its
  //     value.)",
  //     "[JSON][Stringify][Object][XML]")
  //   {
  //     BufferDestination jsonDestination;
  //     json.parse(BufferSource{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" });
  //     json.stringify(jsonDestination);
  //     REQUIRE(jsonDestination.getBuffer() == "l4:True11:Out of timei-2147483648ed3:keyi4444eee");
  //   }
}
