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
    REQUIRE(NRef<Object>(json.root()).size() == 2);
    REQUIRE(NRef<String>((json.root())["City"]).value() == "Southampton");
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
  SECTION("Parse/Print with R-Value reference destination (File).", "[JSON][Node][R-Value-Reference]")
  {
    const std::string generatedFileName{ generateRandomFileName() };
    json.parse(BufferSource{ R"([1,2,3])" });
    JSON::setIndent(4);
    json.print(FileDestination{ generatedFileName });
    REQUIRE_FALSE(JSON::fromFile(generatedFileName).empty());
    std::filesystem::remove(generatedFileName);
    JSON::setIndent(4);
  }
  SECTION("Strip with R-Value reference source.", "[JSON][Node][R-Value-Reference]")
  {
    BufferDestination strippedDestination;
    JSON::strip(BufferSource{ "[1,  2,  3]" }, strippedDestination);
    REQUIRE(strippedDestination.toString() == "[1,2,3]");
  }
  SECTION("Strip with R-Value reference destination.", "[JSON][Node][R-Value-Reference]")
  {
    const std::string generatedFileName{ generateRandomFileName() };
    BufferSource jsonSource{ "[1,  2,  3]" };
    JSON::strip(jsonSource, FileDestination{ generatedFileName });
    REQUIRE(JSON::fromFile(generatedFileName) == "[1,2,3]");
    std::filesystem::remove(generatedFileName);
  }
  SECTION("Strip with both R-Value reference source and destination.", "[JSON][Node][R-Value-Reference]")
  {
    const std::string generatedFileName{ generateRandomFileName() };
    JSON::strip(BufferSource{ "[1,  2,  3]" }, FileDestination{ generatedFileName });
    REQUIRE(JSON::fromFile(generatedFileName) == "[1,2,3]");
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

TEST_CASE("Check standard escape sequence translation.", "[JSON][Translator]")
{
  Default_Translator translator;
  SECTION("Translate from '\\\\' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\\)") == "\\"); }
  SECTION("Translate from '\\t' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\t)") == "\t"); }
  SECTION("Translate from '\\\"' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\")") == "\""); }
  SECTION("Translate from '\\b' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\b)") == "\b"); }
  SECTION("Translate from '\\f' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\f)") == "\f"); }
  SECTION("Translate from '\\n' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\n)") == "\n"); }
  SECTION("Translate from '\\r' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.from(R"(\r)") == "\r"); }
  SECTION("Translate from '\\u0041' escape sequence yields 'A'.", "[JSON][Translator]")
  {
    REQUIRE(translator.from(R"(\u0041)") == "A");
  }
  SECTION("Translate to '\\\\' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\\") == R"(\\)"); }
  SECTION("Translate to '\\t' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\t") == R"(\t)"); }
  SECTION("Translate to '\\\"' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\"") == R"(\")"); }
  SECTION("Translate to '\\b' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\b") == R"(\b)"); }
  SECTION("Translate to '\\f' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\f") == R"(\f)"); }
  SECTION("Translate to '\\n' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\n") == R"(\n)"); }
  SECTION("Translate to '\\r' escape sequence.", "[JSON][Translator]") { REQUIRE(translator.to("\r") == R"(\r)"); }
  SECTION("Translate from/to round-trip leaves plain text unchanged.", "[JSON][Translator]")
  {
    const std::string plain{ "Hello World" };
    REQUIRE(translator.from(plain) == plain);
    REQUIRE(translator.to(plain) == plain);
  }
}

TEST_CASE("Check JSON::version() api.", "[JSON][Version]")
{
  SECTION("version() returns a non-empty string.", "[JSON][Version]") { REQUIRE_FALSE(JSON::version().empty()); }
  SECTION("version() string contains the version number.", "[JSON][Version]")
  {
    REQUIRE(JSON::version().find("1.1.0") != std::string::npos);
  }
}
