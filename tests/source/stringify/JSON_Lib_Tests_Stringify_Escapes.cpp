#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON stringification of strings with escape characters.", "[JSON][Stringify][Escapes]")
{
  const JSON json;
  SECTION("Stringify JSON string with escapes '/' to buffer and check value.", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz / \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    REQUIRE(NRef<String>(json.root()).value() == "abcdefghijklmnopqrstuvwxyz / \t ");
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\t' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\"' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \" ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\\' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \\ ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\b' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \b ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\/' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    // Escaped normal ascii is not re-scaped for stringification
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \/ ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"("abcdefghijklmnopqrstuvwxyz / ")");
  }
  SECTION("Stringify JSON string with escapes '\\f' to buffer and check value.", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ "\"abcdefghijklmnopqrstuvwxyz \\f \"" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\n' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \n ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\r' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \r ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\t' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\u0123' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \u0123 ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(
    R"(Stringify JSON string with escapes '\u0123 \u0456' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \u0123 \u0456 ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\u007F (non-printable ASCII)' to buffer and check value.)",
    "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("abcdefghijklmnopqrstuvwxyz \u007F")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes (1-255)' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    std::string source{ R"("abcdefghijklmnopqrstuvwxyz )" };
    source += generateEscapes(1, 255);
    source += "\"";
    std::string expected{
      R"("abcdefghijklmnopqrstuvwxyz \u0001\u0002\u0003\u0004\u0005\u0006\u0007\b\t\n\u000B\f\r\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\u007F\u0080\u0081\u0082\u0083\u0084\u0085\u0086\u0087\u0088\u0089\u008A\u008B\u008C\u008D\u008E\u008F\u0090\u0091\u0092\u0093\u0094\u0095\u0096\u0097\u0098\u0099\u009A\u009B\u009C\u009D\u009E\u009F\u00A0\u00A1\u00A2\u00A3\u00A4\u00A5\u00A6\u00A7\u00A8\u00A9\u00AA\u00AB\u00AC\u00AD\u00AE\u00AF\u00B0\u00B1\u00B2\u00B3\u00B4\u00B5\u00B6\u00B7\u00B8\u00B9\u00BA\u00BB\u00BC\u00BD\u00BE\u00BF\u00C0\u00C1\u00C2\u00C3\u00C4\u00C5\u00C6\u00C7\u00C8\u00C9\u00CA\u00CB\u00CC\u00CD\u00CE\u00CF\u00D0\u00D1\u00D2\u00D3\u00D4\u00D5\u00D6\u00D7\u00D8\u00D9\u00DA\u00DB\u00DC\u00DD\u00DE\u00DF\u00E0\u00E1\u00E2\u00E3\u00E4\u00E5\u00E6\u00E7\u00E8\u00E9\u00EA\u00EB\u00EC\u00ED\u00EE\u00EF\u00F0\u00F1\u00F2\u00F3\u00F4\u00F5\u00F6\u00F7\u00F8\u00F9\u00FA\u00FB\u00FC\u00FD\u00FE\u00FF")"
    };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ source });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
}