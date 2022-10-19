//
// Unit Tests: JSON_Lib_Tests_Misc
//
// Description: Miscellaneous unit tests for JSON class
// using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSON_Lib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSON_Lib;
// ==========
// Test cases
// ==========
// ===============
// JSON Translator
// ===============
TEST_CASE("Check translation of surrogate pairs.", "[JSON][Translator]")
{
  JSON_Converter converter;
  JSON_Translator translator(converter);
  SECTION("Translate from escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value.",
    "[JSON][Translator]")
  {
    const std::u8string expected{ u8"Begin \U0001D11E End" };
    REQUIRE(translator.fromJSON(R"(Begin \uD834\uDD1E End)") == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 \\uDD1E End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromJSON(R"(Begin \uD834 \uDD1E End)"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(
      translator.fromJSON(R"(Begin \uD834 \uDD1E End)"), "JSON Translator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834\\u0045 End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromJSON(R"(Begin \uD834\u0045 End)"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(
      translator.fromJSON(R"(Begin \uD834\u0045 End)"), "JSON Translator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromJSON(R"(Begin \uD834 End)"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON(R"(Begin \uD834 End)"), "JSON Translator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uDD1E End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromJSON(R"(Begin \uDD1E End)"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON(R"(Begin \uDD1E End)"), "JSON Translator Error: Unpaired surrogate found.");
  }
  SECTION("Translate to escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value.",
    "[JSON][Translator]")
  {
    std::u8string actual{ u8"Begin \U0001D11E End" };
    REQUIRE(translator.toJSON({ actual.begin(), actual.end() }) == R"(Begin \uD834\uDD1E End)");
  }
}
// ========================
// R-Value Reference API(s)
// ========================
TEST_CASE("Check R-Value reference parse/stringify.", "[JSON][JNode][R-Value-Reference]")
{
  const JSON json;
  SECTION("Parse with R-Value reference (Buffer).", "[JSON][JNode][R-Value-Reference]")
  {
    json.parse(BufferSource{ R"({"City":"Southampton","Population":500000 })" });
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).getString() == "Southampton");
  }
  SECTION("Parse/Stringify both with R-Value reference (File).", "[JSON][JNode][R-Value-Reference]")
  {
    const std::string testFileName{ prefixPath(kSingleJSONFile) };
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    json.parse(FileSource{ testFileName });
    json.stringify(FileDestination{ generatedFileName });
    REQUIRE(JSON::fromFile(generatedFileName) == stripWhiteSpace(JSON::fromFile(testFileName)));
  }
}
// =====================
// Strip JSON Whitespace
// =====================
TEST_CASE("Check white space stripping.", "[JSON][Parse][Strip]")
{
  const JSON json;
  SECTION("Strip JSON with containing whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "[1,  \t 2,3,4,5,6,7  ]" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.getBuffer() == "[1,2,3,4,5,6,7]");
  }
  SECTION("Strip JSON with trailing whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "[1,  \t 2,3,4,5,6,7  ]\t  \t" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.getBuffer() == "[1,2,3,4,5,6,7]");
  }
  SECTION("Strip JSON with prefixed whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "\t\t   [1,  \t 2,3,4,5,6,7  ]" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.getBuffer() == "[1,2,3,4,5,6,7]");
  }
  TEST_FILE_LIST(testFile);
  SECTION("Stripped (Buffer) should be the same as parsed then stringified JSON.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ JSON::fromFile(prefixPath(testFile)) };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.getBuffer() == strippedDestination.getBuffer());
  }
  SECTION("Stripped (File) should be the same as parsed then stringified JSON.", "[JSON][Parse][Strip]")
  {
    const std::string generatedFileName{ prefixPath(kGeneratedJSONFile) };
    std::filesystem::remove(generatedFileName);
    FileSource jsonSource{ prefixPath(testFile) };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    FileDestination strippedDestination{ generatedFileName };
    json.strip(jsonSource, strippedDestination);
    strippedDestination.close();
    REQUIRE(jsonDestination.getBuffer() == JSON::fromFile(generatedFileName));
  }
  SECTION("Strip JSON with escaped ascii characters.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ R"(   [  "fffgh \/ \n\t \p \w \u1234 "  ]       )" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.getBuffer() == "[\"fffgh \\/ \\n\\t \\p \\w \\u1234 \"]");
  }
}
