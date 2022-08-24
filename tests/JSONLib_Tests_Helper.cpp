//
// Unit Tests: JSON
//
// Description: JSON unit test helper functions.
//
// ================
// Test definitions
// =================
#include "JSONLib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSONLib;
// ==========================
// Unit test helper functions
// ==========================
/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="jsonFileName">Test JSON data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixPath(const std::string &jsonFileName)
{
  const std::filesystem::path currentPath = std::filesystem::current_path() / "files" / jsonFileName;
  return (currentPath.string());
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns>JSON string.</returns>
std::string readFromFile(const std::string &jsonFileName)
{
  std::ifstream jsonFile;
  jsonFile.open(jsonFileName, std::ios_base::binary);
  std::ostringstream jsonFileBuffer;
  jsonFileBuffer << jsonFile.rdbuf();
  return (jsonFileBuffer.str());
}
/// <summary>
/// Create an JSON file and write JSON.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <param name="jsonString">JSON string</param>
/// <returns></returns>
void writeToFile(const std::string &jsonFileName, const std::string &jsonString)
{
  std::remove(jsonFileName.c_str());
  std::ofstream jsonFile;
  jsonFile.open(jsonFileName, std::ios::binary);
  jsonFile << jsonString;
  jsonFile.close();
}
/// <summary>
/// Verify that an JNode Array has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNode Array</param>
/// <returns></returns>
void checkArray(const JNode &jNode)
{// Array ["Dog",1964,true,null]
  REQUIRE(jNode.getType() == JNodeType::array);
  REQUIRE(JRef<Array>(jNode).size() == 4);
  REQUIRE(jNode[0].getType() == JNodeType::string);
  REQUIRE(jNode[1].getType() == JNodeType::number);
  REQUIRE(jNode[2].getType() == JNodeType::boolean);
  REQUIRE(jNode[3].getType() == JNodeType::null);
  REQUIRE(JRef<String>(jNode[0]).getString() == "Dog");
  REQUIRE(JRef<Number>(jNode[1]).getNumber().getInt() == 1964);
  REQUIRE(JRef<Boolean>(jNode[2]).getBoolean() == true);
  REQUIRE(JRef<Null>(jNode[3]).getNull() == nullptr);
}
/// <summary>
/// Verify that an JNode Object has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNode Object</param>
/// <returns></returns>
void checkObject(const JNode &jNode)
{// {"City":"Southampton","Population":500000}
  REQUIRE(jNode.getType() == JNodeType::object);
  REQUIRE(JRef<Object>(jNode).size() == 2);
  REQUIRE(JRef<Object>(jNode).contains("City"));
  REQUIRE(JRef<Object>(jNode).contains("Population"));
  REQUIRE(jNode["City"].getType() == JNodeType::string);
  REQUIRE(jNode["Population"].getType() == JNodeType::number);
  REQUIRE(JRef<String>(jNode["City"]).getString() == "Southampton");
  REQUIRE(JRef<Number>(jNode["Population"]).getNumber().getInt() == 500000);
}
/// <summary>
/// Strip white space from source JSON and place remainder in destination.
/// </summary>
/// <param name="json">JSON parser object</param>
/// <param name="jsonBuffer">Source json</param>
/// <returns></returns>
std::string stripWhiteSpace(const std::string &jsonBuffer)
{
  const JSONLib::JSON json;
  BufferSource source(jsonBuffer);
  BufferDestination destination;
  json.strip(source, destination);
  return (destination.getBuffer());
}