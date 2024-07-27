#include "JSON_Lib_Tests.hpp"

// ==============================================================
// Stringification of collection types to a buffer and validation
// ==============================================================
TEST_CASE("Check JSON stringification of collection types to a buffer and check values.", "[JSON][Stringify][Buffer]")
{
  const JSON json;
  SECTION(R"(Stringify an object ({"Age":77,"Name":"Rob"}) to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"Age":77,"Name":"Rob"})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an array ([999,"Time",null,true] to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([999,"Time",null,true])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to a buffer.)",
    "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"City":"London","Population":[1,2,3,4,5]})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a buffer.)",
    "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
}
// =============================================================
// Stringification of collection types to a file and validation
// =============================================================
TEST_CASE("Check JSON stringification of collection types to a file and check values.", "[JSON][Stringify][File]")
{
  const JSON json;
  const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
  SECTION(R"(Stringify an object ({"Age":77,"Name":"Rob"}) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"Age":77,"Name":"Rob"})" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION(R"(Stringify an array ([999,"Time",null,true] to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([999,"Time",null,true])" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION(
    R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"City":"London","Population":[1,2,3,4,5]})" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION(R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a file.)",
    "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
}
