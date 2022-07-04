//
// Unit Tests: JSON
//
// Description: JSON stringification unit tests for JSON class
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
// ==============================================
// Stringification of simple types and validation
// ==============================================
TEST_CASE("JSON object for stringification of simple types and check values", "[JSON][Stringify][Simple][Validate]")
{
    JSON json;
    SECTION("Stringify a string (Test string) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"\"Test string.\""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a string (Test another string) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"\"Test another string.\""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a boolean (true) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"true"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a boolean (false) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"false"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a number (98345) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"98345"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a number (250000) and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"250000"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a null and check its value", "[JSON][Stringify][Simple][Validate]")
    {
        std::string expected{"null"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}
// ==================================================
// Stringification of collection types and validation
// ==================================================
TEST_CASE("JSON object for stringification of collection types and check values.", "[JSON][Stringify]")
{
    JSON json;
    SECTION("Stringify an object ({\"Age\":77,\"Name\":\"Rob\"}) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"{\"Age\":77,\"Name\":\"Rob\"}"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([999,\"Time\",null,true] and check its value", "[JSON][Stringify]")
    {
        std::string expected{"[999,\"Time\",null,true]"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an object ({\"City\":\"London\",\"Population\":8000000}) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([true,\"Out of time\",789043e13,true]) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"[true,\"Out of time\",789043e13,true]"};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}
// ================================================
// Stringification to file and validation of result
// ================================================
TEST_CASE("Stringify to a file and check result.", "[JSON][Stringify][File][Validate]")
{
    JSON json;
    const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
    SECTION("Stringify object to file and check value", "[JSON][Stringify][File][Validate]")
    {
        std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
        std::filesystem::remove(generatedFileName);
        FileDestination jsonDestination{generatedFileName};
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(readFromFile(generatedFileName) == expected);
    }
    SECTION("Stringify array to file and check value", "[JSON][Stringify][File][Validate]")
    {
        std::string expected{"[999,\"Time\",null,true]"};
        std::filesystem::remove(generatedFileName);
        FileDestination jsonDestination{generatedFileName};
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(readFromFile(generatedFileName) == expected);
    }
}
// ====================================
// Stringification generated exceptions
// ====================================
TEST_CASE("Stringify generated exceptions", "[JSON][Stringify][Exceptions]")
{
    JSON json;
    SECTION("Stringify has no JSON to process", "[JSON][Stringify][Exceptions]")
    {
        REQUIRE_THROWS_AS(json.stringify(BufferDestination{}), JSONLib::Error);
        REQUIRE_THROWS_WITH(json.stringify(BufferDestination{}), "JSON Error: No JSON to stringify.");
    }
}
// =================================
// Parsing and stringification files
// =================================
TEST_CASE("JSON object for stringification of a list of example JSON files.", "[JSON][Stringify]")
{
    JSON json;
    auto testFile = GENERATE(values<std::string>({"testfile001.json",
                                                  "testfile002.json",
                                                  "testfile003.json",
                                                  "testfile004.json",
                                                  "testfile005.json"}));
    SECTION("Stringify to  buffer and check value", "[JSON][Stringify][Buffer]")
    {
        std::string jsonFileBuffer{readFromFile(prefixTestDataPath(testFile))};
        BufferSource jsonSource{jsonFileBuffer};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == stripWhiteSpace(json, jsonFileBuffer));
    }
    SECTION("Stringify to file and check value", "[JSON][Stringify][File]")
    {
        const std::string testFileName {prefixTestDataPath(testFile)};
        const std::string generatedFileName {prefixTestDataPath(kGeneratedJSONFile)};
        std::filesystem::remove(generatedFileName);
        std::string jsonFileBuffer{readFromFile(testFileName)};
        BufferSource jsonSource{jsonFileBuffer};
        FileDestination jsonDestination{generatedFileName};
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(readFromFile(generatedFileName) == stripWhiteSpace(json, jsonFileBuffer));
    }
}
// =====================================
// Stringification of escaped characters
// =====================================
TEST_CASE("JSON object for stringification of strings with escape characters.", "[JSON][Stringify][Escapes]")
{
    JSON json;
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\t \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\\"' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\\" \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\\\' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\\\ \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\b' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\b \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\/' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\/ \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\f' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\f \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\n' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\n \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\r' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\r \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\t \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\u0123' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\u0123 \""};
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\u0123 \\u0456' to buffer and check value", "[JSON][Stringify][Escapes]")
    {
        std::string expected{"\"Test String \\u0123 \\u0456 \""}; 
        BufferDestination jsonDestination;
        json.parse(BufferSource{expected});
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}