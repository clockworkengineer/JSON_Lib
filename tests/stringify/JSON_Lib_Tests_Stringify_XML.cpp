//
// Unit Tests: JSON_Lib_Tests_XML_Stringify
//
// Description: XML stringification of simple/collection types unit
// tests for JSON class using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"
#include "XML_Stringify.hpp"
#include "XML_Translator.hpp"
#include "JSON_Converter.hpp"

using namespace JSON_Lib;

// ================================================================
// Stringification of simple/collection types to XML and validation
// ================================================================
TEST_CASE("Check JSON stringification to XML of simple types.", "[JSON][Stringify][Simple][XML]")
{
  const JSON json(std::make_unique<XML_Stringify>(XML_Translator(JSON_Converter())).release());
  SECTION(
    "Stringify a string (abcdefghijklmnopqrstuvwxyz) to XML and check its value.", "[JSON][Stringify][String][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("abcdefghijklmnopqrstuvwxyz")" });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz</root>)");
  }
  SECTION(
    "Stringify a string (abcdefghijklmnopqrstuvwxyz &<>'\") to XML with default character escapes and check its value.",
    "[JSON][Stringify][String][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"("abcdefghijklmnopqrstuvwxyz &<>'\"")" });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &amp;&lt;&gt;&apos;&quot;</root>)");
  }
  SECTION(
    "XML encode an string with unprintable characters "
    "('abcdefghijklmnopqrstuvwxyz') and check its value ",
    "[JSON][Stringify][XML][String]")
  {
    std::string escaped;
    escaped += "\"abcdefghijklmnopqrstuvwxyz";
    // escaped += static_cast<char>(0);
    escaped += static_cast<char>(1);
    escaped += static_cast<char>(2);
    escaped += '"';
    BufferSource source{ escaped };
    BufferDestination destination;
    json.parse(source);
    json.stringify(destination);
    REQUIRE(
      destination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0001;&#x0002;</root>)");
  }
  // SECTION(
  //   "XML encode an string with unprintable characters (1-255) and check its value ", "[JSON][Stringify][XML][String]")
  // {
  //   std::string escaped{ "\"abcdefghijklmnopqrstuvwxyz" };
  //   // Add all ASCII except '"' and '\'
  //   for (int ch = 1; ch < 255; ch++) {
  //     if ((static_cast<char>(ch) != '"') && (static_cast<char>(ch) != '\\')) { escaped += static_cast<char>(ch); }
  //   }
  //   escaped += '"';
  //   BufferSource source{ escaped };
  //   BufferDestination destination;
  //   json.parse(source);
  //   json.stringify(destination);
  //   REQUIRE(
  //     destination.toString()
  //     == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0001;&#x0002;&#x0003;&#x0004;&#x0005;&#x0006;&#x0007;&#x0008;&#x0009;&#x000a;&#x000b;&#x000c;&#x000d;&#x000e;&#x000f;&#x0010;&#x0011;&#x0012;&#x0013;&#x0014;&#x0015;&#x0016;&#x0017;&#x0018;&#x0019;&#x001a;&#x001b;&#x001c;&#x001d;&#x001e;&#x001f; !#$%&amp;&apos;()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~&#x007f;&#x0080;&#x0081;&#x0082;&#x0083;&#x0084;&#x0085;&#x0086;&#x0087;&#x0088;&#x0089;&#x008a;&#x008b;&#x008c;&#x008d;&#x008e;&#x008f;&#x0090;&#x0091;&#x0092;&#x0093;&#x0094;&#x0095;&#x0096;&#x0097;&#x0098;&#x0099;&#x009a;&#x009b;&#x009c;&#x009d;&#x009e;&#x009f;&#x00a0;&#x00a1;&#x00a2;&#x00a3;&#x00a4;&#x00a5;&#x00a6;&#x00a7;&#x00a8;&#x00a9;&#x00aa;&#x00ab;&#x00ac;&#x00ad;&#x00ae;&#x00af;&#x00b0;&#x00b1;&#x00b2;&#x00b3;&#x00b4;&#x00b5;&#x00b6;&#x00b7;&#x00b8;&#x00b9;&#x00ba;&#x00bb;&#x00bc;&#x00bd;&#x00be;&#x00bf;&#x00c0;&#x00c1;&#x00c2;&#x00c3;&#x00c4;&#x00c5;&#x00c6;&#x00c7;&#x00c8;&#x00c9;&#x00ca;&#x00cb;&#x00cc;&#x00cd;&#x00ce;&#x00cf;&#x00d0;&#x00d1;&#x00d2;&#x00d3;&#x00d4;&#x00d5;&#x00d6;&#x00d7;&#x00d8;&#x00d9;&#x00da;&#x00db;&#x00dc;&#x00dd;&#x00de;&#x00df;&#x00e0;&#x00e1;&#x00e2;&#x00e3;&#x00e4;&#x00e5;&#x00e6;&#x00e7;&#x00e8;&#x00e9;&#x00ea;&#x00eb;&#x00ec;&#x00ed;&#x00ee;&#x00ef;&#x00f0;&#x00f1;&#x00f2;&#x00f3;&#x00f4;&#x00f5;&#x00f6;&#x00f7;&#x00f8;&#x00f9;&#x00fa;&#x00fb;&#x00fc;&#x00fd;&#x00fe;</root>)");
  // }
  SECTION("Stringify a boolean (true) to XML and check its value.", "[JSON][Stringify][Boolean][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "true" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root>True</root>)");
  }
  SECTION("Stringify a boolean (false) to XML and check its value.", "[JSON][Stringify][Boolean][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "false" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root>False</root>)");
  }
  SECTION("Stringify a integer (98345) to XML and check its value.", "[JSON][Stringify][Integer][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "98345" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root>98345</root>)");
  }
  SECTION("Stringify a floating point (55.6667) to XML and check its value.", "[JSON][Stringify][Float][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ "55.6667" });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root>55</root>)");
  }
  SECTION("Stringify a null to XML and check its value.", "[JSON][Stringify][Simple][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "null" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"(<?xml version="1.0" encoding="UTF-8"?><root></root>)");
  }
  SECTION("Stringify an array to XML and check its value.", "[JSON][Stringify][Array][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { "[1,444,555,666,67.99]" } });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>1</Row><Row>444</Row><Row>555</Row><Row>666</Row><Row>67</Row></root>)");
  }
  SECTION("Stringify an object to XML and check its value.", "[JSON][Stringify][Object][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ { R"({"Age":77,"Name":"Rob"})" } });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString()
            == R"(<?xml version="1.0" encoding="UTF-8"?><root><Age>77</Age><Name>Rob</Name></root>)");
  }
  SECTION(R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to XML and check its value.)",
    "[JSON][Stringify][Array][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"({"City":"London","Population":[1,2,3,4,5]})" });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root><City>London</City><Population><Row>1</Row><Row>2</Row><Row>3</Row><Row>4</Row><Row>5</Row></Population></root>)");
  }
  SECTION(
    R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer and check its
      value.)",
    "[JSON][Stringify][Object][XML]")
  {
    BufferDestination jsonDestination;
    json.parse(BufferSource{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>True</Row><Row>Out of time</Row><Row>-2147483648</Row><Row><key>4444</key></Row></root>)");
  }
  SECTION(R"(Stringify XML string with escapes '\u007F (non-printable ASCII)' to buffer and check value.)",
    "[JSON][Stringify][XML][Escapes]")
  {
    const std::string source{ R"("abcdefghijklmnopqrstuvwxyz \u007F")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString()
            == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &#x007f;</root>)");
  }
  SECTION(R"(Stringify XML string with escapes (1-127)' to buffer and check value.)", "[JSON][Stringify][XML][Escapes]")
  {
    std::string source{ R"("abcdefghijklmnopqrstuvwxyz )" };
    for (char16_t utf16Char = 1; utf16Char < 128; utf16Char++) {
      const char *digits = "0123456789ABCDEF";
      source += "\\u";
      source += digits[(utf16Char >> 12) & 0x0f];
      source += digits[(utf16Char >> 8) & 0x0f];
      source += digits[(utf16Char >> 4) & 0x0f];
      source += digits[(utf16Char)&0x0f];
    }
    source += "\"";
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(
      jsonDestination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &#x0001;&#x0002;&#x0003;&#x0004;&#x0005;&#x0006;&#x0007;&#x0008;&#x0009;&#x000a;&#x000b;&#x000c;&#x000d;&#x000e;&#x000f;&#x0010;&#x0011;&#x0012;&#x0013;&#x0014;&#x0015;&#x0016;&#x0017;&#x0018;&#x0019;&#x001a;&#x001b;&#x001c;&#x001d;&#x001e;&#x001f; !&quot;#$%&amp;&apos;()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~&#x007f;</root>)");
  }
  SECTION(R"(Stringify XML string with escapes '\u0080 (non-printable ASCII)' to buffer and check value.)",
    "[JSON][Stringify][XML][Escapes]")
  {
    const std::string source{ R"("abcdefghijklmnopqrstuvwxyz \u0080")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString()
            == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &#x0080;</root>)");
  }
}
