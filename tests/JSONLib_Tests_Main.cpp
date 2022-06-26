//
// Unit Tests: JSON
//
// Description: ISource, IDestination and JNode index/reference
// unit tests for JSON class using the Catch2 test framework.
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
TEST_CASE("ISource (File) interface.", "[JSON][Parse][ISource]")
{
  SECTION("Create FileSource with testfile001.json.", "[JSON][Parse][ISource]")
  {
    REQUIRE_NOTHROW(FileSource(prefixTestDataPath(kSingleJSONFile)));
  }
  SECTION("Create FileSource with testfile001.json. and positioned on the correct first character", "[JSON][Parse][ISource]")
  {
    FileSource source{FileSource(prefixTestDataPath(kSingleJSONFile))};
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create FileSource with testfile001.json and then check next positions to correct next character", "[JSON][Parse][ISource]")
  {
    FileSource source = FileSource(prefixTestDataPath(kSingleJSONFile));
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\r');
  }
  SECTION("Create FileSource with non existant file.", "[JSON][Parse][ISource][Exception]")
  {
    REQUIRE_THROWS_AS(FileSource(prefixTestDataPath(kNonExistantJSONFile)), JSONLib::ISource::Error);
    REQUIRE_THROWS_WITH(FileSource(prefixTestDataPath(kNonExistantJSONFile)), "ISource Error: File input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource with testfile001.json and then try to read off the end", "[JSON][Parse][ISource][Exception]")
  {
    FileSource source = FileSource(prefixTestDataPath(kSingleJSONFile));
    while (source.more())
    {
      source.next();
    }
    REQUIRE_THROWS_AS(source.next(), JSONLib::ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past end of file.");
  }
}
TEST_CASE("ISource (Buffer) interface. Contains file testfile001.json.", "[JSON][Parse][ISource]")
{
  std::string buffer{readJSONFromFile(prefixTestDataPath(kSingleJSONFile))};
  SECTION("Create BufferSource.", "[JSON][Parse][ISource]")
  {
    REQUIRE_NOTHROW(BufferSource(buffer));
  }
  SECTION("Create BufferSource with testfile001.json and that it is positioned on the correct first character", "[JSON][Parse][ISource]")
  {
    BufferSource source{BufferSource(buffer)};
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '{');
  }
  SECTION("Create BufferSource with testfile001.json and then check next positions to correct next character", "[JSON][Parse][ISource]")
  {
    BufferSource source{BufferSource(buffer)};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE(static_cast<char>(source.current()) == '\r');
  }
  SECTION("Create BufferSource with testfile001.json move past last character, check it and the bytes moved.", "[JSON][Parse][ISource]")
  {
    BufferSource source{BufferSource(buffer)};
    long length = 0;
    while (source.more())
    {
      source.next();
      length++;
    }
    REQUIRE(length == 605);                              // eof
    REQUIRE(source.current() == static_cast<ISource::Char>(EOF)); // eof
  }
  SECTION("Create BufferSource with empty buffer.", "[JSON][Parse][ISource][Exception]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), JSONLib::ISource::Error);
    REQUIRE_THROWS_WITH(BufferSource(""), "ISource Error: Empty source buffer passed to be parsed.");
  }
  SECTION("Create BufferSource with testfile001.json and then try to read off the end", "[JSON][Parse][ISource][Exception]")
  {
    BufferSource source{BufferSource(buffer)};
    while (source.more())
    {
      source.next();
    }
    REQUIRE_THROWS_AS(source.next(), JSONLib::ISource::Error);
    REQUIRE_THROWS_WITH(source.next(), "ISource Error: Tried to read past and of buffer.");
  }
}
TEST_CASE("IDestination (Buffer) interface.", "[JSON][Parse][IDestination]")
{
  SECTION("Create BufferDestination.", "[JSON][Stringify][IDestination]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().empty());
  }
  SECTION("Create BufferDestination and add one character.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION("Create BufferDestination and add an stringified integer and check result.", "[JSON][Stringify][IDestination]")
  {
    BufferDestination buffer;
    buffer.add("65767");
    REQUIRE(buffer.getBuffer().size() == 5);
    REQUIRE(buffer.getBuffer() == ("65767"));
  }
}
TEST_CASE("IDestination (File) interface.", "[JSON][Parse][IDestination]")
{
  SECTION("Create FileDestination.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedJSONFile)));
  }
  SECTION("Create FileDestination when file already exists.", "[JSON][Stringify][IDestination]")
  {
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedJSONFile)));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    REQUIRE_FALSE(!std::filesystem::exists(prefixTestDataPath(kGeneratedJSONFile)));
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    file.add('t');
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an stringified integer and check result.", "[JSON][Stringify][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileDestination file{prefixTestDataPath(kGeneratedJSONFile)};
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedJSONFile)};
    file.add("65767");
    REQUIRE(std::filesystem::file_size(filePath) == 5);
    REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) == "65767");
  }
}
// Implement these for JSON
TEST_CASE("Use of JNode indexing operators", "[JSON][JNode][Index]")
{
  JSON json;
  SECTION("Parse dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    checkObject(json.root());
    REQUIRE(JNodeRef<JNodeString>((json.root())["City"]).string() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())["Population"]).number() == "500000");
  }
  SECTION("Parse list and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[0]).number() == "777");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[1]).number() == "9000");
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse list with embedded dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,{\"City\":\"Southampton\",\"Population\":500000},\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[0]).number() == "777");
    REQUIRE(JNodeRef<JNodeString>((json.root())[1]["City"]).string() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((json.root())[1]["Population"]).number() == "500000");
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())["Cityy"].getNodeType() == JNodeType::object, JNode::Error);
    REQUIRE_THROWS_WITH((json.root())["Cityy"].getNodeType() == JNodeType::object, "JNode Error: Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((json.root())[3].getNodeType() == JNodeType::array, JNode::Error);
    REQUIRE_THROWS_WITH((json.root())[3].getNodeType() == JNodeType::array, "JNode Error: Invalid index used to access array.");
  }
}
TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]")
{
  JSON json;
  SECTION("Integer reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"45500"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>(json.root()).number() == "45500");
  }
  SECTION("String reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"\"0123456789\""};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeString>(json.root()).string() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeArray>(json.root()).size() == 3);
    REQUIRE(JNodeRef<JNodeString>((json.root())[2]).string() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000 }"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeObject>(json.root()).size() == 2);
    REQUIRE(JNodeRef<JNodeString>((json.root())["City"]).string() == "Southampton");
  }
}
TEST_CASE("Check JNodeNumber number conversion", "[JSON][JNode][JNodeNumber]")
{
  JSON json;
  SECTION("Floating point not converted to Integer", "[JSON][JNode][JNodeNumber]")
  {
    long long longValue;
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(json.root()).integer(longValue));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]")
  {
    double doubleValue;
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).floatingpoint(doubleValue));
    REQUIRE(doubleValue == 678.8990);
  }
  SECTION("Integer point converted to Long", "[JSON][JNode][JNodeNumber]")
  {
    long long longValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).integer(longValue));
    REQUIRE(longValue == 78989);
  }
  SECTION("Integer point not converted to double", "[JSON][JNode][JNodeNumber]")
  {
    double doubleValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(json.root()).floatingpoint(doubleValue));
  }
  SECTION("Check  floating point with exponent", "[JSON][JNode][JNodeNumber][Exception")
  {
    BufferSource jsonSource{"78.43e-2"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.e43e-2"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Syntax error detected.");
  }
  SECTION("Check floating point with multiple decimal points", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.5454.545"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSONLib::Error);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON Error: Syntax error detected.");
  }
}
TEST_CASE("Check translation of surrogate pairs", "[JSON][DefaultTranslator]")
{
  JSON_Converter converter;
  JSON_Translator translator(converter);
  SECTION("Translate from escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value", "[JSON][DefaultTranslator]")
  { // Needed to convert const char8_t * to string
    REQUIRE(translator.from("Begin \\uD834\\uDD1E End") == reinterpret_cast<const char *>(u8"Begin \U0001D11E End"));
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
  { // Needed to convert const char8_t * to string
    REQUIRE(translator.to(reinterpret_cast<const char *>(u8"Begin \U0001D11E End")) == "Begin \\uD834\\uDD1E End");
  }
}
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
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    json.parse(FileSource{prefixTestDataPath(kSingleJSONFile)});
    json.stringify(FileDestination{prefixTestDataPath(kGeneratedJSONFile)});
    REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) ==
            stripWhiteSpace(json, (readJSONFromFile(prefixTestDataPath(kSingleJSONFile)))));
  }
}
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
    BufferSource jsonSource{readJSONFromFile(prefixTestDataPath(testFile))};
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
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    FileSource jsonSource{prefixTestDataPath(testFile)};
    BufferDestination jsonDestination;
    json.parse(jsonSource);
    json.stringify(jsonDestination);
    jsonSource.reset();
    FileDestination strippedDestination{prefixTestDataPath(kGeneratedJSONFile)};
    json.strip(jsonSource, strippedDestination);
    REQUIRE(jsonDestination.getBuffer() == readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)));
  }
}
