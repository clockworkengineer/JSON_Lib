//
// Unit Tests: JSON
//
// Description: ISource, IDestination and JNode index/reference
// unit tests for JSON class using the Catch2 test framework.
//
// ================
// Test definitions
// =================
#include "JSON_tests.hpp"
// =======================
// JSON class namespace
// =======================
using namespace JSONLib;
// ==========================
// Unit test helper functions
// ==========================
/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file)
{
  std::filesystem::path currentPath = std::filesystem::current_path() / "testData" / file;
  return (currentPath.string());
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns></returns>
std::string readJSONFromFile(const std::string &jsonFileName)
{
  std::ifstream jsonFile;
  jsonFile.open(jsonFileName);
  std::ostringstream jsonFileBuffer;
  jsonFileBuffer << jsonFile.rdbuf();
  return (jsonFileBuffer.str());
}
/// <summary>
/// Verify that an JNodeArray has the correct pasred format.
/// </summary>
/// <param name="jNode">Pointer to JNNodeArray</param>
/// <returns></returns>
void checkArray(JNode *jNode)
{ // Array [\"Dog\",1964,true,null]
  REQUIRE(jNode->nodeType == JNodeType::array);
  REQUIRE(JNodeRef<JNodeArray>(*jNode).size() == 4);
  REQUIRE((*jNode)[0].nodeType == JNodeType::string);
  REQUIRE((*jNode)[1].nodeType == JNodeType::number);
  REQUIRE((*jNode)[2].nodeType == JNodeType::boolean);
  REQUIRE((*jNode)[3].nodeType == JNodeType::null);
  REQUIRE(JNodeRef<JNodeString>((*jNode)[0]).getString() == "Dog");
  REQUIRE(JNodeRef<JNodeNumber>((*jNode)[1]).getNumber() == "1964");
  REQUIRE(JNodeRef<JNodeBoolean>((*jNode)[2]).getBoolean() == true);
  REQUIRE(JNodeRef<JNodeNull>((*jNode)[3]).getNull() == nullptr);
}
/// <summary>
/// Verify that an JNodeObject has the correct pasred format.
/// </summary>
/// <param name="jNode">Pointer to JNodeObject</param>
/// <returns></returns>
void checkObject(JNode *jNode)
{ // {\"City\":\"Southampton\",\"Population\":500000}
  REQUIRE(jNode->nodeType == JNodeType::object);
  REQUIRE(JNodeRef<JNodeObject>(*jNode).size() == 2);
  REQUIRE(JNodeRef<JNodeObject>(*jNode).containsKey("City"));
  REQUIRE(JNodeRef<JNodeObject>(*jNode).containsKey("Population"));
  REQUIRE((*jNode)["City"].nodeType == JNodeType::string);
  REQUIRE((*jNode)["Population"].nodeType == JNodeType::number);
  REQUIRE(JNodeRef<JNodeString>((*jNode)["City"]).getString() == "Southampton");
  REQUIRE(JNodeRef<JNodeNumber>((*jNode)["Population"]).getNumber() == "500000");
}
// ==========
// Test cases
// ==========
TEST_CASE("ISource (File) interface.", "[JSON][Parse][ISource]")
{
  SECTION("Create FileSource with testfile001.json.", "[JSON][Parse][ISource]")
  {
    REQUIRE_NOTHROW(FileSource(prefixTestDataPath(kSingleJSONFile)));
  }
  SECTION("Create FileSource with non existants file.", "[JSON][Parse][ISource][Exception]")
  {
    REQUIRE_THROWS_AS(FileSource(prefixTestDataPath(kNonExistantJSONFile)), std::runtime_error);
    REQUIRE_THROWS_WITH(FileSource(prefixTestDataPath(kNonExistantJSONFile)), "JSON file input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource with testfile001.json. and positioned on the correct first character", "[JSON][Parse][ISource]")
  {
    FileSource source{FileSource(prefixTestDataPath(kSingleJSONFile))};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '{');
  }
  SECTION("Create FileSource with testfile001.json and then check moveToNextByte positions to correct next character", "[JSON][Parse][ISource]")
  {
    FileSource source = FileSource(prefixTestDataPath(kSingleJSONFile));
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '\n');
  }
}
TEST_CASE("ISource (Buffer interface). Contains file testfile001.json.", "[JSON][Parse][ISource]")
{
  std::string buffer{readJSONFromFile(prefixTestDataPath(kSingleJSONFile))};
  SECTION("Create BufferSource.", "[JSON][Parse][ISource]")
  {
    REQUIRE_NOTHROW(BufferSource(buffer));
  }
  SECTION("Create BufferSource with empty buffer.", "[JSON][Parse][ISource][Exception]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), std::invalid_argument);
    REQUIRE_THROWS_WITH(BufferSource(""), "Empty source buffer passed to be parsed.");
  }
  SECTION("Create BufferSource with testfile001.json and that it is positioned on the correct first character", "[JSON][Parse][ISource]")
  {
    BufferSource source{BufferSource(buffer)};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '{');
  }
  SECTION("Create BufferSource with testfile001.json and then check moveToNextByte positions to correct next character", "[JSON][Parse][ISource]")
  {
    BufferSource source{BufferSource(buffer)};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '\n');
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
    REQUIRE(length == 583);                              // eof
    REQUIRE(source.current() == static_cast<char>(255)); // eof
  }
}
TEST_CASE("IDestination (Buffer interface).", "[JSON][Parse][ISource]")
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
    buffer.add("i");
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
TEST_CASE("IDestination (File interface).", "[JSON][Parse][ISource]")
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
    file.add("t");
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
    checkObject(json.getRoot());
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())["City"]).getString() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())["Population"]).getNumber() == "500000");
  }
  SECTION("Parse list and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[0]).getNumber() == "777");
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[1]).getNumber() == "9000");
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[2]).getString() == "apples");
  }
  SECTION("Parse list with embedded dictionary and check its components using indexing", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,{\"City\":\"Southampton\",\"Population\":500000},\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[0]).getNumber() == "777");
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[1]["City"]).getString() == "Southampton");
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())[1]["Population"]).getNumber() == "500000");
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[2]).getString() == "apples");
  }
  SECTION("Parse dictionary and check an invalid key generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000}"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((*json.getRoot())["Cityy"].nodeType == JNodeType::object, std::runtime_error);
    REQUIRE_THROWS_WITH((*json.getRoot())["Cityy"].nodeType == JNodeType::object, "Invalid key used to access object.");
  }
  SECTION("Parse list and check an invalid index generates exception", "[JSON][JNode][Index]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE_THROWS_AS((*json.getRoot())[3].nodeType == JNodeType::array, std::runtime_error);
    REQUIRE_THROWS_WITH((*json.getRoot())[3].nodeType == JNodeType::array, "Invalid index used to access array.");
  }
}
TEST_CASE("Check JNode reference functions work.", "[JSON][JNode][Reference]")
{
  JSON json;
  SECTION("Integer reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"45500"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeNumber>((*json.getRoot())).getNumber() == "45500");
  }
  SECTION("String reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"0123456789"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())).getString() == "0123456789");
  }
  SECTION("Array reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"[777,9000,\"apples\"]"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeArray>((*json.getRoot())).size() == 3);
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())[2]).getString() == "apples");
  }
  SECTION("Dictionary reference.", "[JSON][JNode][Reference]")
  {
    BufferSource jsonSource{"{\"City\":\"Southampton\",\"Population\":500000 }"};
    json.parse(jsonSource);
    REQUIRE(JNodeRef<JNodeObject>((*json.getRoot())).size() == 2);
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())["City"]).getString() == "Southampton");
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
    REQUIRE_FALSE(JNodeRef<JNodeNumber>(*json.getRoot()).getInteger(longValue));
  }
  SECTION("Floating point converted to double", "[JSON][JNode][JNodeNumber]")
  {
    double doubleValue;
    BufferSource jsonSource{"678.8990"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(*json.getRoot()).getFloatingPoint(doubleValue));
    REQUIRE(doubleValue == 678.8990);
  }
  SECTION("Integer point converted to Long", "[JSON][JNode][JNodeNumber]")
  {
    long long longValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(*json.getRoot()).getInteger(longValue));
    REQUIRE(longValue == 78989);
  }
  SECTION("Integer point not converted to double", "[JSON][JNode][JNodeNumber]")
  {
    double doubleValue;
    BufferSource jsonSource{"78989"};
    json.parse(jsonSource);
    REQUIRE_FALSE(!JNodeRef<JNodeNumber>(*json.getRoot()).getFloatingPoint(doubleValue));
  }
  SECTION("Check  floating point with exponent", "[JSON][JNode][JNodeNumber][Exception")
  {
    BufferSource jsonSource{"78.43e-2"};
    REQUIRE_NOTHROW(json.parse(jsonSource));
  }
  SECTION("Check floating point with invalid exponent", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.e43e-2"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
  }
  SECTION("Check floating point with multiple decimal points", "[JSON][JNode][JNodeNumber][Exception]")
  {
    BufferSource jsonSource{"78.5454.545"};
    REQUIRE_THROWS_AS(json.parse(jsonSource), JSON::SyntaxError);
    jsonSource.reset();
    REQUIRE_THROWS_WITH(json.parse(jsonSource), "JSON syntax error detected.");
  }
}
TEST_CASE("Check translation of surrogate pairs", "[JSON][DefaultTranslator]")
{
  JSONTranslator translator;
  SECTION("Translate from escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value", "[JSON][DefaultTranslator]")
  {
    REQUIRE(translator.fromEscapeSequences("Begin \\uD834\\uDD1E End") == u8"Begin \U0001D11E End");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 \\uDD1E End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromEscapeSequences("Begin \\uD834 \\uDD1E End"), JSON::SyntaxError);
    REQUIRE_THROWS_WITH(translator.fromEscapeSequences("Begin \\uD834 \\uDD1E End"), "JSON syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834\\u0045 End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromEscapeSequences("Begin \\uD834\\u0045 End"), JSON::SyntaxError);
    REQUIRE_THROWS_WITH(translator.fromEscapeSequences("Begin \\uD834\\u0045 End"), "JSON syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uD834 End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromEscapeSequences("Begin \\uD834 End"), JSON::SyntaxError);
    REQUIRE_THROWS_WITH(translator.fromEscapeSequences("Begin \\uD834 End"), "JSON syntax error detected.");
  }
  SECTION("Translate from escape sequences surrogate pair 'Begin \\uDD1E End' in error then expect exception", "[JSON][DefaultTranslator][Exception]")
  {
    REQUIRE_THROWS_AS(translator.fromEscapeSequences("Begin \\uDD1E End"), JSON::SyntaxError);
    REQUIRE_THROWS_WITH(translator.fromEscapeSequences("Begin \\uDD1E End"), "JSON syntax error detected.");
  }
  SECTION("Translate to escape sequences valid surrogate pair 'Begin \\uD834\\uDD1E End' and check value", "[JSON][DefaultTranslator]")
  {
    REQUIRE(translator.toEscapeSequences(u8"Begin \U0001D11E End") == "Begin \\uD834\\uDD1E End");
  }
}
TEST_CASE("Check R-Value reference parse/stringify.", "[JSON][JNode][R-Value Reference]")
{
  JSON json;
  SECTION("Parse with R-Value reference (Buffer).", "[JSON][JNode][R-Value Reference]")
  {
    json.parse(BufferSource{"{\"City\":\"Southampton\",\"Population\":500000 }"});
    REQUIRE(JNodeRef<JNodeObject>((*json.getRoot())).size() == 2);
    REQUIRE(JNodeRef<JNodeString>((*json.getRoot())["City"]).getString() == "Southampton");
  }
  SECTION("Parse/Stringify both with R-Value reference (File).", "[JSON][JNode][R-Value Reference]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedJSONFile));
    json.parse(FileSource{prefixTestDataPath(kSingleJSONFile)});
    json.stringify(FileDestination{prefixTestDataPath(kGeneratedJSONFile)});
    REQUIRE(readJSONFromFile(prefixTestDataPath(kGeneratedJSONFile)) ==
            json.stripWhiteSpaceBuffer(readJSONFromFile(prefixTestDataPath(kSingleJSONFile))));
  }
}