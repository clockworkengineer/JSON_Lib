//
// Unit Tests: JSON_Lib_Tests_Helper
//
// Description: JSON unit test helper functions.
//
// ================
// Test definitions
// =================
#include "JSON_Lib_Tests.hpp"
// ======================
// JSON library namespace
// ======================
using namespace JSON_Lib;
// ==========================
// Unit test helper functions
// ==========================
/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="jsonFileName">Test JSON data file name</param>
/// <returns>Full path to test data file</returns>
const std::string prefixPath(const std::string &jsonFileName)
{
  return ((std::filesystem::current_path() / "files" / jsonFileName).string());
}
/// <summary>
/// Verify that an JNode Array has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to JNode Array</param>
/// <returns></returns>
void checkArray(const JNode &jNode)
{// Array ["Dog",1964,true,null]
  REQUIRE_FALSE(!jNode.isArray());
  REQUIRE(JRef<Array>(jNode).size() == 4);
  REQUIRE_FALSE(!jNode[0].isString());
  REQUIRE_FALSE(!jNode[1].isNumber());
  REQUIRE_FALSE(!jNode[2].isBoolean());
  REQUIRE_FALSE(!jNode[3].isNull());
  REQUIRE(JRef<String>(jNode[0]).getString() == "Dog");
  REQUIRE(JRef<Number>(jNode[1]).get<int>() == 1964);
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
  REQUIRE_FALSE(!jNode.isObject());
  REQUIRE(JRef<Object>(jNode).size() == 2);
  REQUIRE(JRef<Object>(jNode).contains("City"));
  REQUIRE(JRef<Object>(jNode).contains("Population"));
  REQUIRE_FALSE(!jNode["City"].isString());
  REQUIRE_FALSE(!jNode["Population"].isNumber());
  REQUIRE(JRef<String>(jNode["City"]).getString() == "Southampton");
  REQUIRE(JRef<Number>(jNode["Population"]).get<int>() == 500000);
}
/// <summary>
/// Strip white space from source JSON and place remainder in destination.
/// </summary>
/// <param name="json">JSON parser object</param>
/// <param name="jsonBuffer">Source json</param>
/// <returns></returns>
std::string stripWhiteSpace(const std::string &jsonBuffer)
{
  const JSON_Lib::JSON json;
  BufferDestination destination;
  json.strip(BufferSource{ jsonBuffer }, destination);
  return (destination.getBuffer());
}