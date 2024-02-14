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
    REQUIRE(destination.toString()
            == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0001;&#x0002;</root>)");
  }
  SECTION(
    "XML encode an string with unprintable characters (1-127) and check its value ", "[JSON][Stringify][XML][String]")
  {
    std::string escaped{ "\"abcdefghijklmnopqrstuvwxyz" };
    // Add all ASCII except '"' and '\'
    for (int ch = 1; ch < 128; ch++) {
      if ((static_cast<char>(ch) != '"') && (static_cast<char>(ch) != '\\')) { escaped += static_cast<char>(ch); }
    }
    escaped += '"';
    BufferSource source{ escaped };
    BufferDestination destination;
    json.parse(source);
    json.stringify(destination);
    REQUIRE(
      destination.toString()
      == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0001;&#x0002;&#x0003;&#x0004;&#x0005;&#x0006;&#x0007;&#x0008;&#x0009;&#x000A;&#x000B;&#x000C;&#x000D;&#x000E;&#x000F;&#x0010;&#x0011;&#x0012;&#x0013;&#x0014;&#x0015;&#x0016;&#x0017;&#x0018;&#x0019;&#x001A;&#x001B;&#x001C;&#x001D;&#x001E;&#x001F; !#$%&amp;&apos;()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~&#x007F;</root>)");
  }
  SECTION(
    "XML encode an string with unprintable characters (128) and check its value ", "[JSON][Stringify][XML][String]")
  {
    std::string escaped{ "\"abcdefghijklmnopqrstuvwxyz" };
    // Add all ASCII except '"' and '\'
    escaped += 128;
    escaped += '"';
    BufferSource source{ escaped };
    BufferDestination destination;
    json.parse(source);
    json.stringify(destination);
    REQUIRE(destination.toString()
            == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0080;</root>)");
  }
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
            == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &#x007F;</root>)");
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
      == R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &#x0001;&#x0002;&#x0003;&#x0004;&#x0005;&#x0006;&#x0007;&#x0008;&#x0009;&#x000A;&#x000B;&#x000C;&#x000D;&#x000E;&#x000F;&#x0010;&#x0011;&#x0012;&#x0013;&#x0014;&#x0015;&#x0016;&#x0017;&#x0018;&#x0019;&#x001A;&#x001B;&#x001C;&#x001D;&#x001E;&#x001F; !&quot;#$%&amp;&apos;()*+,-./0123456789:;&lt;=&gt;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~&#x007F;</root>)");
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
