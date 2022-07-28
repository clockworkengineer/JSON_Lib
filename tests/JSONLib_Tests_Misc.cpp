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
TEST_CASE("Check translation of surrogate pairs", "[JSON][DefaultTranslator]") {
  JSON_Converter converter;
  JSON_Translator translator(converter);
  SECTION("Translate from escape sequences valid surrogate pair 'Begin "
          "\\uD834\\uDD1E End' and check value",
          "[JSON][DefaultTranslator]") {
    const std::u8string expected{u8"Begin \U0001D11E End"};
    REQUIRE(translator.fromJSON("Begin \\uD834\\uDD1E End") ==
            std::string{expected.begin(), expected.end()});
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 "
          "\\uDD1E End' in error then expect exception",
          "[JSON][DefaultTranslator][Exception]") {
    REQUIRE_THROWS_AS(translator.fromJSON("Begin \\uD834 \\uDD1E End"),
                      JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON("Begin \\uD834 \\uDD1E End"),
                        "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin "
          "\\uD834\\u0045 End' in error then expect exception",
          "[JSON][DefaultTranslator][Exception]") {
    REQUIRE_THROWS_AS(translator.fromJSON("Begin \\uD834\\u0045 End"),
                      JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON("Begin \\uD834\\u0045 End"),
                        "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 End' "
          "in error then expect exception",
          "[JSON][DefaultTranslator][Exception]") {
    REQUIRE_THROWS_AS(translator.fromJSON("Begin \\uD834 End"),
                      JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON("Begin \\uD834 End"),
                        "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uDD1E End' "
          "in error then expect exception",
          "[JSON][DefaultTranslator][Exception]") {
    REQUIRE_THROWS_AS(translator.fromJSON("Begin \\uDD1E End"),
                      JSON_Translator::Error);
    REQUIRE_THROWS_WITH(translator.fromJSON("Begin \\uDD1E End"),
                        "JSON Translator Error: Syntax error detected.");
  }
  SECTION("Translate to escape sequences valid surrogate pair 'Begin "
          "\\uD834\\uDD1E End' and check value",
          "[JSON][DefaultTranslator]") {
    std::u8string actual{u8"Begin \U0001D11E End"};
    REQUIRE(translator.toJSON({actual.begin(), actual.end()}) ==
            "Begin \\uD834\\uDD1E End");
  }
}
// ========================
// R-Value Reference API(s)
// ========================
TEST_CASE("Check R-Value reference parse/stringify.",
          "[JSON][JNode][R-Value Reference]") {
  const JSON json;
  SECTION("Parse with R-Value reference (Buffer).",
          "[JSON][JNode][R-Value Reference]") {
    json.parse(
        BufferSource{"{\"City\":\"Southampton\",\"Population\":500000 }"});
    REQUIRE(JNodeRef<Object>(json.root()).size() == 2);
    REQUIRE(JNodeRef<String>((json.root())["City"]).string() ==
            "Southampton");
  }
  SECTION("Parse/Stringify both with R-Value reference (File).",
          "[JSON][JNode][R-Value Reference]") {
    const std::string testFileName{prefixTestDataPath(kSingleJSONFile)};
    const std::string generatedFileName{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::remove(generatedFileName);
    json.parse(FileSource{testFileName});
    json.stringify(FileDestination{generatedFileName});
    REQUIRE(readFromFile(generatedFileName) ==
            stripWhiteSpace(json, (readFromFile(testFileName))));
  }
}
TEST_CASE("Check whitespace stripping with escape characters.",
          "[JSON][Parse][Strip]") {
  const JSON json;
  SECTION("Strip JSON with escaped ascii characters.", "[JSON][Parse][Strip]") {
    BufferSource jsonSource{
        "   [  \"fffgh \\/ \\n\\t \\p \\w \\u1234 \"  ]       "};
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.getBuffer() ==
            "[\"fffgh / \\n\\t p w \\u1234 \"]");
  }
  TEST_FILE_LIST(testFile);
  SECTION("Stripped (File) should be the same as parsed then stringified JSON",
          "[JSON][Parse][Strip]") {
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
// =====================
// Strip JSON Whitespace
// =====================
TEST_CASE("Check white space stripping.", "[JSON][Parse][Strip]") {
  const JSON json;
  TEST_FILE_LIST(testFile);
  SECTION(
      "Stripped (Buffer) should be the same as parsed then stringified JSON",
      "[JSON][Parse][Strip]") {
    BufferSource jsonSource{readFromFile(prefixTestDataPath(testFile))};
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.getBuffer() == strippedDestination.getBuffer());
  }
  SECTION("Stripped (File) should be the same as parsed then stringified JSON",
          "[JSON][Parse][Strip]") {
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
