//
// Unit Tests: JSON_Lib_Tests_Stringify_Simple
//
// Description: Bencode stringification of simple types unit
// tests for JSON class using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"
#include "Bencode_Stringify.hpp"

using namespace JSON_Lib;

// ========================================================
// Stringification of simple types to Bencode and validation
// ========================================================
TEST_CASE("Check JSON stringification to Bencode of simple types.", "[JSON][Stringify][Simple][Bencode]")
{
  const JSON json(std::make_unique<Bencode_Stringify>().release());
  SECTION("Stringify a string (Test string) to Bencode and check its value.", "[JSON][Stringify][String][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("Test string.")" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "12:Test string.");
  }
  SECTION("Stringify a boolean (true) to Bencode and check its value.", "[JSON][Stringify][Boolean][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "true" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "4:True");
  }
  SECTION("Stringify a boolean (false) to Bencode and check its value.", "[JSON][Stringify][Boolean][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "false" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "5:False");
  }
  SECTION("Stringify a integer (98345) to Bencode and check its value.", "[JSON][Stringify][Integer][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "i98345e");
  }
  SECTION("Stringify a floating point (55.6667) to Bencode and check its value.", "[JSON][Stringify][Float][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "i55e");
  }
  SECTION("Stringify a null to Bencode and check its value.", "[JSON][Stringify][Simple][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "null" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "4:null");
  }
  SECTION("Stringify an array to Bencode and check its value.", "[JSON][Stringify][Array][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[1,444,555,666,67.99]" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "li1ei444ei555ei666ei67ee");
  }
  SECTION("Stringify an object to Bencode and check its value.", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ {R"({"Age":77,"Name":"Rob"})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == "d3:Agei77e4:Name3:Robe");
  }
}
