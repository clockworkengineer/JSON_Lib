//
// Unit Tests: JSON
//
// Description: JSON print of JSON unit tests for JSON class
// using the Catch2 test framework.
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
// ==========================================
// Pretty printing of sample JSON to a buffer
// ==========================================
TEST_CASE("Check printing of example JSON to a buffer.", "[JSON][Print][Buffer]")
{
  const JSON json;
  SECTION("Print a simple array to a buffer", "[JSON][Print][Array][Buffer]")
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
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a simple object to a buffer", "[JSON][Print][Object][Buffer]")
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
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a nested array to a buffer", "[JSON][Print][Array][Buffer]")
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
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a nested object to a buffer", "[JSON][Print][Object][Buffer]")
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
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
  SECTION("Print a complex JSON object to a buffer", "[JSON][Print][Object][Buffer]")
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
    REQUIRE(jsonDestination.getBuffer() == expected);
  }
}
// ========================================
// Pretty printing of sample JSON to a file
// ========================================
TEST_CASE("Check printing of example JSON to a file.", "[JSON][Print][File]")
{
  const JSON json;
  SECTION("Print a simple array to a file", "[JSON][Print][Array][File]")
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
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION("Print a simple object to a file", "[JSON][Print][Object][File]")
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
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION("Print a nested array to a file", "[JSON][Print][Array][File]")
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
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION("Print a nested object to a file", "[JSON][Print][Object][File]")
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
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
  SECTION("Print a complex JSON object to a file", "[JSON][Print][Object][File]")
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
    REQUIRE(readFromFile(generatedFileName) == expected);
  }
}