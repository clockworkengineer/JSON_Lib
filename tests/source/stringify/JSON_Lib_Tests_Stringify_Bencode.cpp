#include "JSON_Lib_Tests.hpp"
#include "Bencode_Stringify.hpp"

TEST_CASE("Check JSON stringification to Bencode of simple types.", "[JSON][Stringify][Simple][Bencode]")
{
  const JSON json(makeStringify<Bencode_Stringify>());
  SECTION("Stringify a string (abcdefghijklmnopqrstuvwxyz) to Bencode.", "[JSON][Stringify][String][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("abcdefghijklmnopqrstuvwxyz")" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "26:abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Stringify a boolean (true) to Bencode.", "[JSON][Stringify][Boolean][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "true" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "4:True");
  }
  SECTION("Stringify a boolean (false) to Bencode.", "[JSON][Stringify][Boolean][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "false" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "5:False");
  }
  SECTION("Stringify a integer (98345) to Bencode.", "[JSON][Stringify][Integer][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "i98345e");
  }
  SECTION("Stringify a integer (-98345) to Bencode.", "[JSON][Stringify][Integer][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "-98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "i-98345e");
  }
  SECTION("Stringify a floating point (55.6667) to Bencode.", "[JSON][Stringify][Float][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "i55e");
  }
  SECTION("Stringify a floating point (-55.6667) to Bencode.", "[JSON][Stringify][Float][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "-55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "i-55e");
  }
  SECTION("Stringify a null to Bencode.", "[JSON][Stringify][Simple][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "null" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "4:null");
  }
  SECTION("Stringify an array to Bencode.", "[JSON][Stringify][Array][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[1,444,555,666,67.99]" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "li1ei444ei555ei666ei67ee");
  }
  SECTION("Stringify an empty array to Bencode.", "[JSON][Stringify][Array][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[]" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "le");
  }
  SECTION("Stringify an object to Bencode.", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { R"({"Age":77,"Name":"Rob"})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "d3:Agei77e4:Name3:Robe");
  }
  SECTION("Stringify an empty object to Bencode.", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { R"({})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "de");
  }
  SECTION(R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to Bencode.)",
    "[JSON][Stringify][Array][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"({"City":"London","Population":[1,2,3,4,5]})" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "d4:City6:London10:Populationli1ei2ei3ei4ei5eee");
  }
  SECTION(R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a Bencode.)",
    "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "l4:True11:Out of timei7890430242211233792ed3:keyi4444eee");
  }
  SECTION(R"(Stringify [{},{},{}] to a Bencode.)", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([{},{},{}])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "ldededee");
  }
  SECTION(R"(Stringify [[], [],[]] to a Bencode.)", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([[], [],[]])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "llelelee");
  }
  SECTION(R"(Stringify {"Test" : [[],[],[]]} to a Bencode.)", "[JSON][Stringify][Object][Bencode]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"({"Test" : [[],[],[]]})" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "d4:Testlleleleee");
  }
}
