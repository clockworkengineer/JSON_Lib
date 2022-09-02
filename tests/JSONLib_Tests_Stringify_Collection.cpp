//
// Unit Tests: JSONLib
//
// Description: JSON stringification of collection types unit
// tests for JSON class using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSONLib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
// ==============================================================
// Stringification of collection types to a buffer and validation
// ==============================================================
TEST_CASE("JSON object for stringification of collection types to a buffer and check values.",
  "[JSON][Stringify][Buffer]")
{
  const JSON json;
  SECTION(
    R"(Stringify an object ({"Age":77,"Name":"Rob"}) to a buffer and check its value.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"Age":77,"Name":"Rob"})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify an array ([999,"Time",null,true] to a buffer and check its value.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([999,"Time",null,true])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(R"(Stringify an nested object ({"City":"London","Population":[1,2,3,4,5]}) to a buffer and check its value.)",
    "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"City":"London","Population":[1,2,3,4,5]})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION(
    R"(Stringify a nested array ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer and check its value.)",
    "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}
// ============================================================
// Stringification of collection types to a buffer and validation
// ============================================================
TEST_CASE("JSON object for stringification of collection types to a file and check values.", "[JSON][Stringify][File]")
{
  const JSON json;
  const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
  SECTION(R"(Stringify an object ({"Age":77,"Name":"Rob"}) to a file and check its value.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"Age":77,"Name":"Rob"})" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION(R"(Stringify an array ([999,"Time",null,true] to a file and check its value.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([999,"Time",null,true])" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION(R"(Stringify an nested object ({"City":"London","Population":[1,2,3,4,5]}) to a file and check its value.)",
    "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"City":"London","Population":[1,2,3,4,5]})" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION(R"(Stringify a nested array ([true,"Out of time",7.89043e+18,{"key":4444}]) to a file and check its value.)",
    "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
}
