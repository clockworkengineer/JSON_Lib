
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
    REQUIRE(jsonDestination.toString() == "---\n\"Age\": 77\n\"Name\": \"Rob\"\n...\n");
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
      jsonDestination.toString() == "---\n\"City\": \"London\"\n\"Population\": \n  - 1\n  - 2\n  - 3\n  - 4\n  - 5\n...\n");
  }
  SECTION(
    R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer and check its
      value.)",
    "[JSON][Stringify][Object][YAML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n- true\n- \"Out of time\"\n- 7.89043e+18\n- \"key\": 4444\n...\n");
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
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz \\u0001\\u0002\\u0003\\u0004\\u0005\\u0006\\u0007\\u0008\\u0009\\u000A\\u000B\\u000C\\u000D\\u000E\\u000F\\u0010\\u0011\\u0012\\u0013\\u0014\\u0015\\u0016\\u0017\\u0018\\u0019\\u001A\\u001B\\u001C\\u001D\\u001E\\u001F !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\\u007F\"\n...\n");
  }
  SECTION(R"(Stringify YAML string with escapes '\u0080 (non-printable ASCII)' to buffer and check value.)",
    "[JSON][Stringify][YAML][Escapes]")
  {
    const std::string source{ R"("abcdefghijklmnopqrstuvwxyz \u0080")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz \\u0080\"\n...\n");
  }
  SECTION(
    R"(Stringify YAML string with escapes (128-255)' to buffer and check value.)", "[JSON][Stringify][YAML][Escapes]")
  {
    std::string source{ R"("abcdefghijklmnopqrstuvwxyz )" };
    source += generateEscapes(128, 255);
    source += "\"";
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz \\u0080\\u0081\\u0082\\u0083\\u0084\\u0085\\u0086\\u0087\\u0088\\u0089\\u008A\\u008B\\u008C\\u008D\\u008E\\u008F\\u0090\\u0091\\u0092\\u0093\\u0094\\u0095\\u0096\\u0097\\u0098\\u0099\\u009A\\u009B\\u009C\\u009D\\u009E\\u009F\\u00A0\\u00A1\\u00A2\\u00A3\\u00A4\\u00A5\\u00A6\\u00A7\\u00A8\\u00A9\\u00AA\\u00AB\\u00AC\\u00AD\\u00AE\\u00AF\\u00B0\\u00B1\\u00B2\\u00B3\\u00B4\\u00B5\\u00B6\\u00B7\\u00B8\\u00B9\\u00BA\\u00BB\\u00BC\\u00BD\\u00BE\\u00BF\\u00C0\\u00C1\\u00C2\\u00C3\\u00C4\\u00C5\\u00C6\\u00C7\\u00C8\\u00C9\\u00CA\\u00CB\\u00CC\\u00CD\\u00CE\\u00CF\\u00D0\\u00D1\\u00D2\\u00D3\\u00D4\\u00D5\\u00D6\\u00D7\\u00D8\\u00D9\\u00DA\\u00DB\\u00DC\\u00DD\\u00DE\\u00DF\\u00E0\\u00E1\\u00E2\\u00E3\\u00E4\\u00E5\\u00E6\\u00E7\\u00E8\\u00E9\\u00EA\\u00EB\\u00EC\\u00ED\\u00EE\\u00EF\\u00F0\\u00F1\\u00F2\\u00F3\\u00F4\\u00F5\\u00F6\\u00F7\\u00F8\\u00F9\\u00FA\\u00FB\\u00FC\\u00FD\\u00FE\\u00FF\"\n...\n");
  }
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
    REQUIRE(jsonDestination.toString() == "---\n\"Test\": \n  - []\n  - []\n  - []\n...\n");
  }
}
