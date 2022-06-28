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
// =======================
// JSON class namespace
// =======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
TEST_CASE("JSON object for stringification of simple types and check values", "[JSON][Stringify]")
{
    JSON json;
    SECTION("Stringify a string (Test string) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test string.\""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a string (Test another string) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test another string.\""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a boolean (true) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"true"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a boolean (false) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"false"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a number (98345) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"98345"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a number (250000) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"250000"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify a null and check its value", "[JSON][Stringify]")
    {
        std::string expected{"null"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}
TEST_CASE("JSON object for stringification of collection types and check values.", "[JSON][Stringify]")
{
    JSON json;
    SECTION("Stringify an object ({\"Age\":77,\"Name\":\"Rob\"}) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"{\"Age\":77,\"Name\":\"Rob\"}"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([999,\"Time\",null,true] and check its value", "[JSON][Stringify]")
    {
        std::string expected{"[999,\"Time\",null,true]"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an object ({\"City\":\"London\",\"Population\":8000000}) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify an array ([true,\"Out of time\",789043e13,true]) and check its value", "[JSON][Stringify]")
    {
        std::string expected{"[true,\"Out of time\",789043e13,true]"};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}
TEST_CASE("Stringify to a file and check result.", "[JSON][Stringify][Exceptions]")
{
    JSON json;
    SECTION("Stringify object to file and check value", "[JSON][Stringify]")
    {
        std::string expected{"{\"City\":\"London\",\"Population\":8000000}"};
        std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
        BufferSource jsonSource{expected};
        FileDestination jsonDestination{prefixTestDataPath(kGeneratedJSONFile)};
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) == expected);
    }
    SECTION("Stringify array to file and check value", "[JSON][Stringify]")
    {
        std::string expected{"[999,\"Time\",null,true]"};
        std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
        BufferSource jsonSource{expected};
        FileDestination jsonDestination{prefixTestDataPath(kGeneratedJSONFile)};
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) == expected);
    }
}
TEST_CASE("Stringify generated exceptions", "[JSON][Stringify][Exceptions]")
{
    JSON json;
    SECTION("Stringify has no JSON to process", "[JSON][Stringify][Exceptions]")
    {
        REQUIRE_THROWS_AS(json.stringify(BufferDestination {}), JSONLib::Error);
        REQUIRE_THROWS_WITH(json.stringify(BufferDestination {}), "JSON Error: No JSON to stringify.");
    }
// //     SECTION("Stringify passed invalid JNode type", "[JSON][Stringify][Exceptions]")
// //     {
// //         REQUIRE_THROWS_AS(json.stringifyToBuffer(JNode::Ptr(new JNode())), std::runtime_error);
// //         REQUIRE_THROWS_WITH(json.stringifyToBuffer(JNode::Ptr(new JNode())), "Unknown JNode type encountered during stringification.");
// //     }
// //     SECTION("Stringify file passed invalid JNode type", "[JSON][Stringify][Exceptions]")
// //     {
// //         REQUIRE_THROWS_AS(json.stringifyToFile(JNode::Ptr(nullptr), prefixTestDataPath(kGeneratedJSONFile)), std::invalid_argument);
// //         REQUIRE_THROWS_WITH(json.stringifyToFile(JNode::Ptr(nullptr), prefixTestDataPath(kGeneratedJSONFile)), "Nullptr passed as JNode root to be stringified.");
// //     }
// //     SECTION("Stringify file passed empty string for file name", "[JSON][Stringify][Exceptions]")
// //     {
// //         REQUIRE_THROWS_AS(json.stringifyToFile(JNode::Ptr(new JNode()), ""), std::invalid_argument);
// //         REQUIRE_THROWS_WITH(json.stringifyToFile(JNode::Ptr(new JNode()), ""), "Empty file name passed to be stringified.");
// //     }
}
TEST_CASE("JSON object for stringification of a list of example JSON files.", "[JSON][Stringify]")
{
    JSON json;
    auto testFile = GENERATE(values<std::string>({"testfile001.json",
                                                  "testfile002.json",
                                                  "testfile003.json",
                                                  "testfile004.json",
                                                  "testfile005.json"}));
    SECTION("Stringify to  buffer and check value", "[JSON][Stringify]")
    {
        std::string jsonFileBuffer{readJSONFromFile(prefixTestDataPath(testFile))};
        BufferSource jsonSource{jsonFileBuffer};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == stripWhiteSpace(json,jsonFileBuffer));
    }
    SECTION("Stringify to file and check value", "[JSON][Stringify]")
    {
        std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
        std::string jsonFileBuffer{readJSONFromFile(prefixTestDataPath(testFile))};
        BufferSource jsonSource{jsonFileBuffer};
        FileDestination jsonDestination{prefixTestDataPath(kGeneratedJSONFile)};
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) == stripWhiteSpace(json, jsonFileBuffer));
    }
}
TEST_CASE("JSON object for stringification of strings with escape characters.", "[JSON][Stringify]")
{
    JSON json;
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\t \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\\"' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\\" \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\\\' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\\\ \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\b' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\b \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\/' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\/ \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\f' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\f \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\n' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\n \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\r' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\r \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\t \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\u0123' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\u0123 \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
    SECTION("Stringify JSON string with escapes '\\u0123 \\u0456' to buffer and check value", "[JSON][Stringify]")
    {
        std::string expected{"\"Test String \\u0123 \\u0456 \""};
        BufferSource jsonSource{expected};
        BufferDestination jsonDestination;
        json.parse(jsonSource);
        json.stringify(jsonDestination);
        REQUIRE(jsonDestination.getBuffer() == expected);
    }
}