#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check translation of surrogate pairs.", "[JSON][Translator]")
{
  Default_Translator translator;
  SECTION("Translate from escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value.",
    "[JSON][Translator]")
  {
    const std::u8string expected{ u8"Begin \U0001D11E End" };
    REQUIRE(translator.from(R"(Begin \uD834\uDD1E End)") == std::string{ expected.begin(), expected.end() });
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 \\uDD1E End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from(R"(Begin \uD834 \uDD1E End)"), Default_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from(R"(Begin \uD834 \uDD1E End)"), "ITranslator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834\\u0045 End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from(R"(Begin \uD834\u0045 End)"), Default_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from(R"(Begin \uD834\u0045 End)"), "ITranslator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from(R"(Begin \uD834 End)"), Default_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from(R"(Begin \uD834 End)"), "ITranslator Error: Unpaired surrogate found.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uDD1E End' in error then expect exception.",
    "[JSON][Translator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.from(R"(Begin \uDD1E End)"), Default_Translator::Error);
    REQUIRE_THROWS_WITH(translator.from(R"(Begin \uDD1E End)"), "ITranslator Error: Unpaired surrogate found.");
  }
  SECTION("Translate to escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value.",
    "[JSON][Translator]")
  {
    std::u8string_view actual{ u8"Begin \U0001D11E End" };
    REQUIRE(translator.to(std::string(actual.begin(), actual.end())) == R"(Begin \uD834\uDD1E End)");
  }
}

TEST_CASE("Check R-Value reference parse/stringify.", "[JSON][Node][R-Value-Reference]")
{
  const JSON json;
  SECTION("Parse with R-Value reference (Buffer).", "[JSON][Node][R-Value-Reference]")
  {
    json.parse(BufferSource{ R"({"City":"Southampton","Population":500000 })" });
    REQUIRE(JRef<Object>(json.root()).size() == 2);
    REQUIRE(JRef<String>((json.root())["City"]).value() == "Southampton");
  }
  SECTION("Parse/Stringify both with R-Value reference (File).", "[JSON][Node][R-Value-Reference]")
  {
    const std::string testFileName{ prefixTestDataPath(kSingleJSONFile) };
    const std::string generatedFileName{ generateRandomFileName() };
    json.parse(FileSource{ testFileName });
    json.stringify(FileDestination{ generatedFileName });
    REQUIRE(JSON::fromFile(generatedFileName) == stripWhiteSpace(JSON::fromFile(testFileName)));
    std::filesystem::remove(generatedFileName);
  }
}

TEST_CASE("Check white space stripping.", "[JSON][Parse][Strip]")
{
  const JSON json;
  SECTION("Strip JSON with containing whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "[1,  \t 2,3,4,5,6,7  ]" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.toString() == "[1,2,3,4,5,6,7]");
  }
  SECTION("Strip JSON with trailing whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "[1,  \t 2,3,4,5,6,7  ]\t  \t" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.toString() == "[1,2,3,4,5,6,7]");
  }
  SECTION("Strip JSON with prefixed whitespace.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ "\t\t   [1,  \t 2,3,4,5,6,7  ]" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.toString() == "[1,2,3,4,5,6,7]");
  }
  TEST_FILE_LIST(testFile);
  SECTION("Stripped (Buffer) should be the same as parsed then stringified JSON.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ JSON::fromFile(prefixTestDataPath(testFile)) };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.size() == strippedDestination.size());
  }
  SECTION("Stripped (File) should be the same as parsed then stringified JSON.", "[JSON][Parse][Strip]")
  {
    const std::string generatedFileName{ generateRandomFileName() };
    FileSource jsonSource{ prefixTestDataPath(testFile) };
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    FileDestination strippedDestination{ generatedFileName };
    json.strip(jsonSource, strippedDestination);
    strippedDestination.close();
    REQUIRE(jsonDestination.toString() == JSON::fromFile(generatedFileName));
    std::filesystem::remove(strippedDestination.getFileName());
  }
  SECTION("Strip JSON with escaped ascii characters.", "[JSON][Parse][Strip]")
  {
    BufferSource jsonSource{ R"(   [  "fffgh \/ \n\t \p \w \u1234 "  ]       )" };
    BufferDestination strippedDestination;
    json.strip(jsonSource, strippedDestination);
    REQUIRE(strippedDestination.toString() == "[\"fffgh \\/ \\n\\t \\p \\w \\u1234 \"]");
  }
}
