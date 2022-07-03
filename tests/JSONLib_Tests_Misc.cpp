//
// Unit Tests: JSON
//
// Description: Miscellaneous unit tests for JSON class
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
// ===============
// JSON Translator
// ===============
TEST_CASE("Check translation of surrogate pairs", "[JSON][DefaultTranslator]")
{
  JSON_Converter converter;
  JSON_Translator translator(converter);
  SECTION("Translate from escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value", "[JSON][DefaultTranslator]")
  {
    std::u8string expected{u8"Begin \U0001D11E End"};
    REQUIRE(translator.from("Begin \\uD834\\uDD1E End") == std::string{expected.begin(), expected.end()});
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 \\uDD1E End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from("Begin \\uD834 \\uDD1E End"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from("Begin \\uD834 \\uDD1E End"), "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834\\u0045 End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from("Begin \\uD834\\u0045 End"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from("Begin \\uD834\\u0045 End"), "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from("Begin \\uD834 End"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from("Begin \\uD834 End"), "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uDD1E End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from("Begin \\uDD1E End"), JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from("Begin \\uDD1E End"), "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate to escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value", "[JSON][DefaultTranslator]")
  {
    std::u8string actual{u8"Begin \U0001D11E End"};
    REQUIRE(translator.to({actual.begin(), actual.end()}) == "Begin \\uD834\\uDD1E End");
  }
}
// ========================
// R-Value Reference API(s)
// ========================
TEST_CASE("Check R-Value reference parse/stringify.", "[JSON][JNode][R-Value Reference]")
{
  JSON json;
  SECTION("Parse with R-Value reference (Buffer).", "[JSON][JNode][R-Value Reference]")
  {
    json.parse(BufferSource{"{\"City\":\"Southampton\",\"Population\":500000 }"});
    REQUIRE(JNodeRef<JNodeObject>(json.root()).size() == 2);
    REQUIRE(JNodeRef<JNodeString>((json.root())["City"]).string() == "Southampton");
  }
  SECTION("Parse/Stringify both with R-Value reference (File).", "[JSON][JNode][R-Value Reference]")
  {
    const std::string testFileName{prefixTestDataPath(kSingleJSONFile)};
    const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::remove(generatedFileName);
    json.parse(FileSource{testFileName});
    json.stringify(FileDestination{generatedFileName});
    REQUIRE(readFromFile(generatedFileName) == stripWhiteSpace(json, (readFromFile(testFileName))));
  }
}
// =====================
// Strip JSON Whitespace
// =====================
TEST_CASE("Check white space stripping.", "[JSON][Parse][Strip]")
{
  JSON json;
  auto testFile = GENERATE(values<std::string>({"testfile001.json",
                                                "testfile002.json",
                                                "testfile003.json",
                                                "testfile004.json",
                                                "testfile005.json"}));
  SECTION("Stripped (Buffer) should be the same as parsed then stringified JSON", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{readFromFile(prefixTestDataPath(testFile))};
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.getBuffer() == strippedDestination.getBuffer());
  }
  SECTION("Stripped (File) should be the same as parsed then stringified JSON", "[JSON][Parse][Strip]")
  {
    const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::remove(generatedFileName);
    FileSource jsonSource{prefixTestDataPath(testFile)};
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    FileDestination strippedDestination{generatedFileName};
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.getBuffer() == readFromFile(generatedFileName));
  }
}
