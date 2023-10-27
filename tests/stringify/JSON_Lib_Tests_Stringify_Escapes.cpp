//
// Unit Tests: JSON_Lib_Tests_Stringify_Escapes
//
// Description: JSON stringification of escapeds characters
// unit tests for JSON class using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;


// =====================================
// Stringification of escaped characters
// =====================================
TEST_CASE("Check JSON stringification of strings with escape characters.", "[JSON][Stringify][Escapes]")
{
  const JSON json;
  SECTION("Stringify JSON string with escapes '/' to buffer and check value.", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String / \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    REQUIRE(JRef<String>(json.root()).getString() == "Test String / \t ");
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\t' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\"' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \" ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\\' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \\ ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\b' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \b ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\/' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    // Escaped normal ascii is not re-scaped for stringification
    const std::string expected{ R"("Test String \/ ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == R"("Test String / ")");
  }
  SECTION("Stringify JSON string with escapes '\\f' to buffer and check value.", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ "\"Test String \\f \"" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\n' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \n ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\r' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \r ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\t' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \t ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify JSON string with escapes '\u0123' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \u0123 ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(
    R"(Stringify JSON string with escapes '\u0123 \u0456' to buffer and check value.)", "[JSON][Stringify][Escapes]")
  {
    const std::string expected{ R"("Test String \u0123 \u0456 ")" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}