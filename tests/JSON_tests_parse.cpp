//
// Unit Tests: JSON
//
// Description: JSON parse unit tests for JSON class
// using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSON_tests.hpp"
// =======================
// JSON class namespace
// =======================
using namespace JSONLib;
// ==========
// Test cases
// ==========
TEST_CASE("JSON object for parse of simple types (number,string,boolean,null).", "[JSON][Parse]")
{
    JSON json;
    SECTION("Parse an string (example string)", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"example string\""};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::string);
    }
    SECTION("Parse an number (6767)", "[JSON][Parse]")
    {
        BufferSource jsonSource{"6767"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::number);
    }
    SECTION("Parse an boolean (true)", "[JSON][Parse]")
    {
        BufferSource jsonSource{"true"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::boolean);
    }
    SECTION("Parse an boolean (false)", "[JSON][Parse]")
    {
        BufferSource jsonSource{"false"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::boolean);
    }
    SECTION("Parse an null", "[JSON][Parse]")
    {
        BufferSource jsonSource{"null"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::null);
    }
}
TEST_CASE("JSON object for parse of simple types and check values.", "[JSON][Parse]")
{
    JSON json;
    SECTION("Parse an string (example string) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"example string\""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "example string");
    }
    SECTION("Parse an string (another example string) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"another example string\""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "another example string");
    }
    SECTION("Parse an number (6767) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"6767"};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeNumber>(*json.getRoot()).getNumber() == "6767");
    }
    SECTION("Parse an number (190000) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"190000"};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeNumber>(*json.getRoot()).getNumber() == "190000");
    }
    SECTION("Parse an boolean (true) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"true"};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeBoolean>(*json.getRoot()).getBoolean() == true);
    }
    SECTION("Parse an boolean (false) and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"false"};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeBoolean>(*json.getRoot()).getBoolean() == false);
    }
    SECTION("Parse an null and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"null"};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeNull>(*json.getRoot()).getNull() == nullptr);
    }
}
TEST_CASE("JSON object for parse of collection types (array,object) ", "[JSON][Parse]")
{
    JSON json;
    SECTION("Parse an object ({\"name\":\"Robert\",\"Age\":15})", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{\"name\":\"Robert\",\"Age\":15}"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::object);
    }
    SECTION("Parse an array ([ 777,9000,\"apples\"]) ", "[JSON][Parse]")
    {
        BufferSource jsonSource{"[777,9000,\"apples\"]"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::array);
    }
}
TEST_CASE("JSON object for parse of collection types and check values", "[JSON][Parse]")
{
      JSON json;
    SECTION("Parse an object {\"name\":\"Robert\",\"Age\":15} and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{\"Name\":\"Robert\",\"Age\":15}"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::object);
        REQUIRE(JNodeRef<JNodeObject>(*json.getRoot()).size() == 2);
        REQUIRE(JNodeRef<JNodeObject>(*json.getRoot()).containsKey("Name"));
        REQUIRE(JNodeRef<JNodeObject>(*json.getRoot()).containsKey("Age"));
        REQUIRE(JNodeRef<JNodeString>((*json.getRoot())["Name"]).nodeType == JNodeType::string);
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())["Age"]).nodeType == JNodeType::number);
        REQUIRE(JNodeRef<JNodeString>((*json.getRoot())["Name"]).getString() == "Robert");
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())["Age"]).getNumber() == "15");
    }
    SECTION("Parse an array [777,9000,\"apples\"] and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"[777,9000,\"apples\"]"};
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::array);
        REQUIRE(JNodeRef<JNodeArray>(*json.getRoot()).size() == 3);
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[0]).nodeType == JNodeType::number);
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[1]).nodeType == JNodeType::number);
        REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[2]).nodeType == JNodeType::string);
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[0]).getNumber() == "777");
        REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[1]).getNumber() == "9000");
        REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[2]).getString() == "apples");
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
    SECTION("Parse an array [\"Dog\",1964,true,null ] and check its value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"[\"Dog\",1964,true,null]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
}
TEST_CASE("JSON object for parse checking various whitespace characters are ignored.", "[JSON][Parse]")
{
      JSON json;
    std::string ws = "";
    SECTION("Parse an array [\"Dog\",1964,true,null] with no whitespace.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with no whitespace", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
    ws += " ";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' '.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' '.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
    ws += "\t";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' \\t'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
    ws += "\n";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t\\n'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} whitespace ' \\t\\n'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
    ws += "\r";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t\\n\\r'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.getRoot());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' \\t\\n\\r'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.getRoot());
    }
}
TEST_CASE("JSON object for parse of a list of example JSON files.", "[JSON][Parse]")
{
      JSON json;
    auto testFile = GENERATE(values<std::string>({"./testData/testfile001.json",
                                                  "./testData/testfile002.json",
                                                  "./testData/testfile003.json",
                                                  "./testData/testfile004.json",
                                                  "./testData/testfile005.json"}));

    SECTION("Parse from buffer", "[JSON][Parse]")
    {
        BufferSource jsonSource{readJSONFromFile(testFile)};
        REQUIRE_NOTHROW(json.parse(jsonSource));
        jsonSource.reset();
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::object);
    }
    SECTION("Parse from file directly", "[JSON][Parse]")
    {
        FileSource jsonSource {testFile};
        REQUIRE_NOTHROW(json.parse(jsonSource));
        jsonSource.reset();
        json.parse(jsonSource);
        REQUIRE(json.getRoot()->nodeType == JNodeType::object);
    }
}
TEST_CASE("Parse generated exceptions.", "[JSON][Parse][Exceptions]")
{
      JSON json;
    // Note: The tests for syntax errors is not exhaustive and more may be added over time.
    // JSON json;
    // SECTION("Parse passing a empty string", "[JSON][Parse]")
    // {
    //     REQUIRE_THROWS_AS(json.parseBuffer(""), std::invalid_argument);
    //     REQUIRE_THROWS_WITH(json.parseBuffer(""), "Empty string passed to be parsed.");
    // }
    // SECTION("Parse file passing a empty file name", "[JSON][Parse]")
    // {
    //     REQUIRE_THROWS_AS(json.parseFile(""), std::invalid_argument);
    //     REQUIRE_THROWS_WITH(json.parseFile(""), "Empty file name passed to be parsed.");
    // }
    SECTION("Parse missing terminating '\"' in string", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{ \"one\" : \"Apple }"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
    SECTION("Parse number with starting with invalid character", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{ \"one\" : z19034}"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
    SECTION("Parse object with invalid value field (number).", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{ \"one\" : 18987u3 }"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
    SECTION("Parse object with missing value field.", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{ \"one\" : }"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
    SECTION("Parse object with missing key field.", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{  : 89012 }"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
    SECTION("Parse object with missing closing '}'.", "[JSON][Parse]")
    {
        BufferSource jsonSource{"{  \"one\" : 18987"};
        REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
        jsonSource.reset();
        REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
    }
}
TEST_CASE("JSON object for decoding of strings with escape characters.", "[JSON][Parse]")
{
      JSON json;
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\t \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \t ");
    }
    SECTION("Stringify JSON string with escapes '\\\"' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\\" \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \" ");
    }
    SECTION("Stringify JSON string with escapes '\\\\' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\\\ \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \\ ");
    }
    SECTION("Stringify JSON string with escapes '\\b' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\b \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \b ");
    }
    SECTION("Stringify JSON string with escapes '\\/' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\/ \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String / ");
    }
    SECTION("Stringify JSON string with escapes '\\f' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\f \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \f ");
    }
    SECTION("Stringify JSON string with escapes '\\n' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\n \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \n ");
    }
    SECTION("Stringify JSON string with escapes '\\r' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\r \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \r ");
    }
    SECTION("Stringify JSON string with escapes '\\t' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\t \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == "Test String \t ");
    }
    SECTION("Stringify JSON string with escapes '\\u0123' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\u0123 \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == u8"Test String \u0123 ");
    }
    SECTION("Stringify JSON string with escapes '\\u0123 \\u0456' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String \\u0123 \\u0456 \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == u8"Test String \u0123 \u0456 ");
    }
    SECTION("Stringify JSON string with escapes  '\\uD834\\uDD1E' to buffer and check value", "[JSON][Parse]")
    {
        BufferSource jsonSource{"\"Test String  \\uD834\\uDD1E \""};
        json.parse(jsonSource);
        REQUIRE(JNodeRef<JNodeString>(*json.getRoot()).getString() == u8"Test String  \U0001D11E ");
    }
}