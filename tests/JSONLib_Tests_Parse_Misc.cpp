//
// Unit Tests: JSON
//
// Description: JSON parse miscellaneous unit tests for JSON class
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
// ========================
// Parse Example JSON Files
// ========================
TEST_CASE("JSON object for parse of a list of example JSON files.", "[JSON][Parse][Examples]")
{
    JSON json;
    auto testFile = GENERATE(values<std::string>({"testfile001.json",
                                                  "testfile002.json",
                                                  "testfile003.json",
                                                  "testfile004.json",
                                                  "testfile005.json"}));

    SECTION("Parse from buffer", "[JSON][Parse][Examples][Buffer]")
    {
        BufferSource jsonSource{readFromFile(prefixTestDataPath(testFile))};
        REQUIRE_NOTHROW(json.parse(jsonSource));
        jsonSource.reset();
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::object);
    }
    SECTION("Parse from file directly", "[JSON][Parse][Examples][File]")
    {
        FileSource jsonSource{prefixTestDataPath(testFile)};
        REQUIRE_NOTHROW(json.parse(jsonSource));
        jsonSource.reset();
        json.parse(jsonSource);
        REQUIRE((json.root()).getNodeType() == JNodeType::object);
    }
}
