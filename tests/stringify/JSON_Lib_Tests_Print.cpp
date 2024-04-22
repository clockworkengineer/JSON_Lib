//
// Unit Tests: JSON_Lib_Tests_Print
//
// Description: JSON pretty print unit tests for JSON class using
// the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

// ==========================================
// Pretty printing of sample JSON to a buffer
// ==========================================
TEST_CASE("Check Check printing of JSON to a buffer.", "[JSON][Print][Buffer]")
{
  const JSON json;
  SECTION("Print a simple array to a buffer.", "[JSON][Print][Array][Buffer]")
  {
    const std::string expected{ R"([
    1,
    2,
    3,
    4,
    5
])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Print a simple object to a buffer.", "[JSON][Print][Object][Buffer]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male"
})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Print a nested array to a buffer.", "[JSON][Print][Array][Buffer]")
  {
    const std::string expected{ R"([
    1,
    2,
    3,
    [
        5,
        6,
        7,
        8
    ],
    5,
    6,
    [
        4,
        5,
        6,
        7,
        8
    ]
])" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Print a nested object to a buffer.", "[JSON][Print][Object][Buffer]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male",
    "Details": {
        "Phone": "0999-999-999",
        "email": "john.doe@express.com",
        "enabled": true
    }
})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Print a complex JSON object to a buffer.", "[JSON][Print][Object][Buffer]")
  {
    const std::string expected{ R"({
    "glossary": {
        "title": "example glossary",
        "GlossDiv": {
            "title": "S",
            "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                    "SortAs": "SGML",
                    "GlossTerm": "Standard Generalized Markup Language",
                    "Acronym": "SGML",
                    "Abbrev": "ISO 8879:1986",
                    "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                        "GlossSeeAlso": [
                            "GML",
                            "XML"
                        ]
                    },
                    "GlossSee": "markup"
                }
            }
        }
    }
})" };
    BufferDestination jsonDestination;
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
}
// ========================================
// Pretty printing of sample JSON to a file
// ========================================
TEST_CASE("Check Check printing of JSON to a file.", "[JSON][Print][File]")
{
  const JSON json;
  SECTION("Print a simple array to a file.", "[JSON][Print][Array][File]")
  {
    const std::string expected{ R"([
    1,
    2,
    3,
    4,
    5
])" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Print a simple object to a file.", "[JSON][Print][Object][File]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male"
})" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Print a nested array to a file.", "[JSON][Print][Array][File]")
  {

    const std::string expected{ R"([
    1,
    2,
    3,
    [
        5,
        6,
        7,
        8
    ],
    5,
    6,
    [
        4,
        5,
        6,
        7,
        8
    ]
])" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Print a nested object to a file.", "[JSON][Print][Object][File]")
  {
    const std::string expected{ R"({
    "name": "Alann",
    "Age": 58,
    "Eye Color": "Blue",
    "Sex": "Male",
    "Details": {
        "Phone": "0999-999-999",
        "email": "john.doe@express.com",
        "enabled": true
    }
})" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Print a complex JSON object to a file.", "[JSON][Print][Object][File]")
  {
    const std::string expected{ R"({
    "glossary": {
        "title": "example glossary",
        "GlossDiv": {
            "title": "S",
            "GlossList": {
                "GlossEntry": {
                    "ID": "SGML",
                    "SortAs": "SGML",
                    "GlossTerm": "Standard Generalized Markup Language",
                    "Acronym": "SGML",
                    "Abbrev": "ISO 8879:1986",
                    "GlossDef": {
                        "para": "A meta-markup language, used to create markup languages such as DocBook.",
                        "GlossSeeAlso": [
                            "GML",
                            "XML"
                        ]
                    },
                    "GlossSee": "markup"
                }
            }
        }
    }
})" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
}
// =========================
// Pretty printing indenting
// =========================
TEST_CASE("Check Check setting of print indentation.", "[JSON][Print][Indent]")
{
  const JSON json;
  SECTION("Set ident to 8 and print JSON to a buffer.", "[JSON][Print][Ident][Buffer]")
  {
    const std::string expected{ R"({
        "name": "Alann",
        "Age": 58,
        "Eye Color": "Blue",
        "Sex": "Male",
        "Details": {
                "Phone": "0999-999-999",
                "email": "john.doe@express.com",
                "enabled": true
        }
})" };
    BufferDestination jsonDestination;
    json.setIndent(8);
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Set ident to 8 and print JSON to a file.", "[JSON][Print][Ident][File]")
  {
    const std::string expected{ R"({
        "name": "Alann",
        "Age": 58,
        "Eye Color": "Blue",
        "Sex": "Male",
        "Details": {
                "Phone": "0999-999-999",
                "email": "john.doe@express.com",
                "enabled": true
        }
})" };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileDestination jsonDestination{ generatedFileName };
    json.setIndent(8);
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == expected);
  }
  SECTION("Set ident to 0 and print JSON to a buffer (result same as stringify).", "[JSON][Print][Ident]")
  {
    const std::string expected{
      R"({"name":"Alann","Age":58,"Eye Color":"Blue","Sex":"Male","Details":{"Phone":"0999-999-999","email":"john.doe@express.com","enabled":true}})"
    };
    BufferDestination jsonDestination;
    json.setIndent(0);
    json.parse(BufferSource{ expected });
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == expected);
  }
  SECTION("Set ident to -4 and trigger invalid indent expection.", "[JSON][Print][Ident][Exception]")
  {
    REQUIRE_THROWS_AS(json.setIndent(-4), JSON_Lib::Error);
    REQUIRE_THROWS_WITH(json.setIndent(-4), "JSON Error: Invalid print indentation value.");
  }
}
// =======================
// Parsing and print files
// =======================
TEST_CASE("Check JSON print of list of example JSON files.", "[JSON][Print]")
{
  const JSON json;
  TEST_FILE_LIST(testFile);
  SECTION("Print to  buffer and check value.", "[JSON][Print][Buffer]")
  {
    json.setIndent(4);
    const std::string jsonFileBuffer{ JSON::fromFile(prefixPath(testFile)) };
    BufferSource jsonSource{ jsonFileBuffer };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.print(jsonDestination);
    REQUIRE(jsonDestination.toString() == jsonFileBuffer);
  }
  SECTION("Print to file and check value.", "[JSON][Print][File]")
  {
    json.setIndent(4);
    const std::string testFileName{ prefixPath(testFile) };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    std::string jsonFileBuffer{ JSON::fromFile(testFileName) };
    BufferSource jsonSource{ jsonFileBuffer };
    FileDestination jsonDestination{ generatedFileName };
    json.parse(jsonSource);
    json.print(jsonDestination);
    jsonDestination.close();
    REQUIRE(JSON::fromFile(generatedFileName) == jsonFileBuffer);
  }
}