#include "JSON_Lib_Tests.hpp"

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
  SECTION(R"(Stringify an empty object ({}) to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an empty array ([]) to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify a deeply nested object ({"a":{"b":{"c":1}}}) to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"a":{"b":{"c":1}}})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an array of objects ([{"id":1,"name":"Alice"},{"id":2,"name":"Bob"}]) to a buffer.)",
    "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([{"id":1,"name":"Alice"},{"id":2,"name":"Bob"}])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an object with all primitive types to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"({"bool":true,"int":42,"null":null,"str":"hello"})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION(R"(Stringify an array of arrays ([[1,2],[3,4],[5,6]]) to a buffer.)", "[JSON][Stringify][Buffer]")
  {
    const std::string expected{ R"([[1,2],[3,4],[5,6]])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
}

TEST_CASE("Check JSON stringification of collection types to a file and check values.", "[JSON][Stringify][File]")
{
  const JSON json;
  SECTION(R"(Stringify an object ({"Age":77,"Name":"Rob"}) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"Age":77,"Name":"Rob"})" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify an array ([999,"Time",null,true] to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([999,"Time",null,true])" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(
    R"(Stringify an nested array ({"City":"London","Population":[1,2,3,4,5]}) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"City":"London","Population":[1,2,3,4,5]})" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify a nested object ([true,"Out of time",7.89043e+18,{"key":4444}]) to a file.)",
    "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([true,"Out of time",7.89043e+18,{"key":4444}])" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify an empty object ({}) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({})" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify an empty array ([]) to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([])" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify an array of objects ([{"id":1,"name":"Alice"},{"id":2,"name":"Bob"}]) to a file.)",
    "[JSON][Stringify][File]")
  {
    const std::string expected{ R"([{"id":1,"name":"Alice"},{"id":2,"name":"Bob"}])" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
  SECTION(R"(Stringify an object with all primitive types to a file.)", "[JSON][Stringify][File]")
  {
    const std::string expected{ R"({"bool":true,"int":42,"null":null,"str":"hello"})" };
    FileDestination jsonDestination{ generateRandomFileName() };
    json.parse(BufferSource{ expected });
    json.stringify(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(jsonDestination.getFileName()) == expected);
    std::filesystem::remove(jsonDestination.getFileName());
  }
}
