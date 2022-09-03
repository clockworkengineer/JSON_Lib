//
// Unit Tests: JSONLib_Tests_Parse_Whitespace
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
TEST_CASE("Check JSON parsing of various whitespace characters are ignored.", "[JSON][Parse][Whitespace]")
{
  const JSON json;
  std::string ws = "";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with no whitespace.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} with no whitespace.)",
    "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += " ";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' '.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(
    R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' '.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\t";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' \t'.)",
    "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\n";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t\n'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(
    R"(Parse object {"City":"Southampton","Population":500000} whitespace. ' \t\n'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
  ws += "\r";
  SECTION(R"(Parse an array ["Dog",1964,true,null] with whitespace ' \t\n\r'.)", "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "[" + ws + R"("Dog")" + ws + "," + ws + "1964" + ws + "," + ws + "true" + ws + ","
                             + ws + "null" + ws + "]" };
    json.parse(jsonSource);
    checkArray(json.root());
  }
  SECTION(R"(Parse object {"City":"Southampton","Population":500000} with whitespace ' \t\n\r'.)",
    "[JSON][Parse][Whitespace]")
  {
    BufferSource jsonSource{ ws + "{" + ws + R"("City")" + ws + ":" + ws + R"("Southampton")" + ws + "," + ws
                             + R"("Population")" + ws + ":" + ws + "500000" + ws + "}" };
    json.parse(jsonSource);
    checkObject(json.root());
  }
}