
#include "JSON_Lib_Tests.hpp"
#include "YAML_Stringify.hpp"

TEST_CASE("Check JSON stringification to YAML of simple types.", "[JSON][Stringify][Simple][YAML]")
{
  const JSON json(makeStringify<YAML_Stringify>());
  SECTION("Stringify a string (abcdefghijklmnopqrstuvwxyz) to YAML.", "[JSON][Stringify][String][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("abcdefghijklmnopqrstuvwxyz")" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz\"\n...\n");
  }
  SECTION(
    "Stringify a string with unprintable characters "
    "('abcdefghijklmnopqrstuvwxyz') ",
    "[JSON][Stringify][YAML][String]")
  {
    std::string escaped;
    escaped += "\"abcdefghijklmnopqrstuvwxyz";
    escaped += 1;
    escaped += 2;
    escaped += '"';
    BufferSource source{ escaped };
    BufferDestination destination;
    json.parse(source);
    json.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz\\u0001\\u0002\"\n...\n");
  }
  SECTION("Stringify an string with unprintable characters (1-127) ", "[JSON][Stringify][YAML][String]")
  {
    std::string escaped{ "\"abcdefghijklmnopqrstuvwxyz" };
    // Add all ASCII except '"' and '\'
    for (int ch = 1; ch < 128; ch++) {
      if (static_cast<char>(ch) != '"' && static_cast<char>(ch) != '\\') { escaped += static_cast<char>(ch); }
    }
    escaped += '"';
    BufferSource source{ escaped };
    BufferDestination destination;
    json.parse(source);
    json.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz\\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\u0008\\u0009\\u000A\\u000B\\u000C\\u000D\\u000E\\u000F\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017\\u0018\\u0019\\u001A\\u001B\\u001C\\u001D\\u001E\\u001F !#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~\\u007F\"\n...\n");
  }
  // // Disable tests for present
  // // SECTION("YAML encode an string with unprintable characters (128-255) ", "[JSON][Stringify][YAML][String]")
  // // {
  // //   std::string escaped{ "\"abcdefghijklmnopqrstuvwxyz" };
  // //   // Add all ASCII except '"' and '\'
  // //   for (int ch = 128; ch < 256; ch++) {
  // //     if ((static_cast<char>(ch) != '"') && (static_cast<char>(ch) != '\\')) { escaped += static_cast<char>(ch); }
  // //   }
  // //   escaped += '"';
  // //   BufferSource source{ escaped };
  // //   BufferDestination destination;
  // //   json.parse(source);
  // //   json.stringify(destination);
  // //   REQUIRE(
  // //     destination.toString()
  // //     == R"(<?xml version="1.0"
  // encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;&#xFFFD;</root>)");
  // //   // 0xFFFD means no such character in UTF8 (generated for 128-255; but can use syntax \u0080 etc).
  // // }
  SECTION("Stringify a boolean (true) to YAML.", "[JSON][Stringify][Boolean][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "true" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\ntrue\n...\n");
  }
  SECTION("Stringify a boolean (false) to YAML.", "[JSON][Stringify][Boolean][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "false" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\nfalse\n...\n");
  }
  SECTION("Stringify a integer (98345) to YAML.", "[JSON][Stringify][Integer][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n98345\n...\n");
  }
  SECTION("Stringify a integer (-98345) to YAML.", "[JSON][Stringify][Integer][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "-98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n-98345\n...\n");
  }
  SECTION("Stringify a floating point (55.6667) to YAML.", "[JSON][Stringify][Float][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n55.6667\n...\n");
  }
  SECTION("Stringify a floating point (-55.6667) to YAML.", "[JSON][Stringify][Float][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "-55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n-55.6667\n...\n");
  }
  SECTION("Stringify a null to YAML.", "[JSON][Stringify][Simple][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "null" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\nnull\n...\n");
  }
  SECTION("Stringify an array to YAML.", "[JSON][Stringify][Array][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[1,444,555,666,67.99]" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n- 1\n- 444\n- 555\n- 666\n- 67.99\n...\n");
  }
  SECTION("Stringify an empty array to YAML.", "[JSON][Stringify][Array][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[]" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n[]\n...\n");
  }
  SECTION("Stringify an object to YAML.", "[JSON][Stringify][Object][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { R"({"Age":77,"Name":"Rob"})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\nAge: 77\nName: \"Rob\"\n...\n");
  }
  SECTION("Stringify an empty object to YAML.", "[JSON][Stringify][Object][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { R"({})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n{}\n...\n");
  }
  SECTION(R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to YAML.)",
    "[JSON][Stringify][Array][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"({"City":"London","Population":[1,2,3,4,5]})" });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString() == "---\nCity: \"London\"\nPopulation: \n  - 1\n  - 2\n  - 3\n  - 4\n  - 5\n...\n");
  }
  SECTION(
    R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer and check its
      value.)",
    "[JSON][Stringify][Object][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n- true\n- \"Out of time\"\n- 7.89043e+18\n- key: 4444\n...\n");
  }
  SECTION(R"(Stringify YAML string with escapes '\u007F (non-printable ASCII)' to buffer and check value.)",
    "[JSON][Stringify][YAML][Escapes]")
  {
    const std::string source{ R"("abcdefghijklmnopqrstuvwxyz \u007F")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz \\u007F\"\n...\n");
  }
  SECTION(
    R"(Stringify YAML string with escapes (1-127)' to buffer and check value.)", "[JSON][Stringify][YAML][Escapes]")
  {
    std::string source{ R"("abcdefghijklmnopqrstuvwxyz )" };
    source += generateEscapes(1, 127);
    source += "\"";
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz \\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\u0008\\u0009\\u000A\\u000B\\u000C\\u000D\\u000E\\u000F\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017\\u0018\\u0019\\u001A\\u001B\\u001C\\u001D\\u001E\\u001F !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\\u007F\"\n...\n");
  }
  // SECTION(R"(Stringify YAML string with escapes '\u0080 (non-printable ASCII)' to buffer and check value.)",
  //   "[JSON][Stringify][YAML][Escapes]")
  // {
  //   const std::string source{ R"("abcdefghijklmnopqrstuvwxyz \u0080")" };
  //   BufferDestination jsonDestination;
  //   json.parse(BufferSource{ source });
  //   json.stringify(jsonDestination);
  //   REQUIRE(jsonDestination.toString() == "");
  // }
  // SECTION(
  //   R"(Stringify YAML string with escapes (128-255)' to buffer and check value.)",
  //   "[JSON][Stringify][YAML][Escapes]")
  // {
  //   std::string source{ R"("abcdefghijklmnopqrstuvwxyz )" };
  //   source += generateEscapes(128, 255);
  //   source += "\"";
  //   BufferDestination jsonDestination;
  //   json.parse(BufferSource{ source });
  //   json.stringify(jsonDestination);
  //   REQUIRE(jsonDestination.toString() == "");
  // }
  SECTION(R"(Stringify [{},{},{}] to a YAML.)", "[JSON][Stringify][Array][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([{},{},{}])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n- {}\n- {}\n- {}\n...\n");
  }
  SECTION(R"(Stringify [[], [],[]] to a YAML.)", "[JSON][Stringify][Array][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([[], [],[]])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n- []\n- []\n- []\n...\n");
  }
  SECTION(R"(Stringify {"Test" : [[],[],[]]} to a YAML.)", "[JSON][Stringify][Object][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"({"Test" : [[],[],[]]})" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\nTest: \n  - []\n  - []\n  - []\n...\n");
  }
}
