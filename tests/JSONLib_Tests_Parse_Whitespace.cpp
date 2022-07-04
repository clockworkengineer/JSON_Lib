//
// Unit Tests: JSON
//
// Description: JSON parse whitespace unit tests for JSON class
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
// ==========================
// Parse JSON with Whitespace
// ==========================
TEST_CASE("JSON object for parse checking various whitespace characters are ignored.", "[JSON][Parse][Whitespace]")
{
    JSON json;
    std::string ws = "";
    SECTION("Parse an array [\"Dog\",1964,true,null] with no whitespace.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.root());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with no whitespace", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
    ws += " ";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' '.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.root());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' '.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
    ws += "\t";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.root());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' \\t'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
    ws += "\n";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t\\n'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.root());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} whitespace ' \\t\\n'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
    ws += "\r";
    SECTION("Parse an array [\"Dog\",1964,true,null] with whitespace ' \\t\\n\\r'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "[" + ws + "\"Dog\"" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + "," + ws + "null" + ws + "]"};
        json.parse(jsonSource);
        checkArray(json.root());
    }
    SECTION("Parse object {\"City\":\"Southampton\",\"Population\":500000} with whitespace ' \\t\\n\\r'.", "[JSON][Parse][Whitespace]")
    {
        BufferSource jsonSource{ws + "{" + ws + "\"City\"" + ws + ":" + ws + "\"Southampton\"" + ws + "," + ws + "\"Population\"" + ws + ":" + ws + "500000" + ws + "}"};
        json.parse(jsonSource);
        checkObject(json.root());
    }
}