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
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file)
{
  const std::filesystem::path currentPath = std::filesystem::current_path() / "testData" / file;
  return (currentPath.string());
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns></returns>
std::string readFromFile(const std::string &jsonFileName)
{
  std::ifstream jsonFile;
  jsonFile.open(jsonFileName, std::ios_base::binary);
  std::ostringstream jsonFileBuffer;
  jsonFileBuffer << jsonFile.rdbuf();
  return (jsonFileBuffer.str());
}
/// <summary>
/// Verify that an JNodeArray has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNNodeArray</param>
/// <returns></returns>
void checkArray(const JNode &jNode)
{ // Array [\"Dog\",1964,true,null]
  REQUIRE(jNode.getNodeType() == JNodeType::array);
  REQUIRE(JNodeRef<JNodeArray>(jNode).size() == 4);
  REQUIRE(jNode[0].getNodeType() == JNodeType::string);
  REQUIRE(jNode[1].getNodeType() == JNodeType::number);
  REQUIRE(jNode[2].getNodeType() == JNodeType::boolean);
  REQUIRE(jNode[3].getNodeType() == JNodeType::null);
  REQUIRE(JNodeRef<JNodeString>(jNode[0]).string() == "Dog");
  REQUIRE(JNodeRef<JNodeNumber>(jNode[1]).number() == "1964");
  REQUIRE(JNodeRef<JNodeBoolean>(jNode[2]).boolean() == true);
  REQUIRE(JNodeRef<JNodeNull>(jNode[3]).null() == nullptr);
}
/// <summary>
/// Verify that an JNodeObject has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNodeObject</param>
/// <returns></returns>
void checkObject(const JNode &jNode)
{ // {\"City\":\"Southampton\",\"Population\":500000}
  REQUIRE(jNode.getNodeType() == JNodeType::object);
  REQUIRE(JNodeRef<JNodeObject>(jNode).size() == 2);
  REQUIRE(JNodeRef<JNodeObject>(jNode).contains("City"));
  REQUIRE(JNodeRef<JNodeObject>(jNode).contains("Population"));
  REQUIRE(jNode["City"].getNodeType() == JNodeType::string);
  REQUIRE(jNode["Population"].getNodeType() == JNodeType::number);
  REQUIRE(JNodeRef<JNodeString>(jNode["City"]).string() == "Southampton");
  REQUIRE(JNodeRef<JNodeNumber>(jNode["Population"]).number() == "500000");
}
/// <summary>
/// Strip white space from source JSON and place remainder in destination.
/// </summary>
/// <param name="json">JSON parser object</param>
/// <param name="jsonBuffer">Source json</param>
/// <returns></returns>
std::string stripWhiteSpace(JSON &json, const std::string &jsonBuffer)
{
  BufferSource source(jsonBuffer);
  BufferDestination destination;
  json.strip(source, destination);
  return (destination.getBuffer());
}