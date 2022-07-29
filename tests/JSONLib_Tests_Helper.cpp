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
std::string prefixTestDataPath(const std::string &jsonFileName) {
  const std::filesystem::path currentPath =
      std::filesystem::current_path() / "files" / jsonFileName;
  return (currentPath.string());
}
/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="jsonFileName">JSON file name</param>
/// <returns>JSON string.</returns>
std::string readFromFile(const std::string &jsonFileName) {
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
/// Verify that an JNodeArrayData has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNNodeArray</param>
/// <returns></returns>
void checkArray(const JNode &jNode) { // Array [\"Dog\",1964,true,null]
  REQUIRE(jNode.getType() == JNodeType::array);
  REQUIRE(JNodeRef<Array>(jNode).size() == 4);
  REQUIRE(jNode[0].getType() == JNodeType::string);
  REQUIRE(jNode[1].getType() == JNodeType::number);
  REQUIRE(jNode[2].getType() == JNodeType::boolean);
  REQUIRE(jNode[3].getType() == JNodeType::null);
  REQUIRE(JNodeRef<String>(jNode[0]).string() == "Dog");
  REQUIRE(JNodeRef<Number>(jNode[1]).toString() == "1964");
  REQUIRE(JNodeRef<Boolean>(jNode[2]).boolean() == true);
  REQUIRE(JNodeRef<Null>(jNode[3]).null() == nullptr);
}
/// <summary>
/// Verify that an JNodeObjectData has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNodeObjectData</param>
/// <returns></returns>
void checkObject(
    const JNode &jNode) { // {\"City\":\"Southampton\",\"Population\":500000}
  REQUIRE(jNode.getType() == JNodeType::object);
  REQUIRE(JNodeRef<Object>(jNode).size() == 2);
  REQUIRE(JNodeRef<Object>(jNode).contains("City"));
  REQUIRE(JNodeRef<Object>(jNode).contains("Population"));
  REQUIRE(jNode["City"].getType() == JNodeType::string);
  REQUIRE(jNode["Population"].getType() == JNodeType::number);
  REQUIRE(JNodeRef<String>(jNode["City"]).string() == "Southampton");
  REQUIRE(JNodeRef<Number>(jNode["Population"]).toString() == "500000");
}
/// <summary>
/// Strip white space from source JSON and place remainder in destination.
/// </summary>
/// <param name="json">JSON parser object</param>
/// <param name="jsonBuffer">Source json</param>
/// <returns></returns>
std::string stripWhiteSpace(const JSON &json, const std::string &jsonBuffer) {
  BufferSource source(jsonBuffer);
  BufferDestination destination;
  json.strip(source, destination);
  return (destination.getBuffer());
}