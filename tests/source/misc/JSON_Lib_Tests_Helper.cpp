#include "JSON_Lib_Tests.hpp"

/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="jsonFileName">Test JSON data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &jsonFileName) {
  if (std::filesystem::is_directory("./files")) {
    return (std::filesystem::current_path() / "./files" / jsonFileName)
        .string();
  } else {
    return (std::filesystem::current_path() / "../files" / jsonFileName)
        .string();
  }
}
/// <summary>
/// Verify that an Node Array has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to Node Array</param>
/// <returns></returns>
void checkArray(const Node &jNode)
{// Array ["Dog",1964,true,null]
  REQUIRE_FALSE(!isA<Array>(jNode));
  REQUIRE(NRef<Array>(jNode).size() == 4);
  REQUIRE_FALSE(!isA<String>(jNode[0]));
  REQUIRE_FALSE(!isA<Number>(jNode[1]));
  REQUIRE_FALSE(!isA<Boolean>(jNode[2]));
  REQUIRE_FALSE(!isA<Null>(jNode[3]));
  REQUIRE(NRef<String>(jNode[0]).value() == "Dog");
  REQUIRE(NRef<Number>(jNode[1]).value<int>() == 1964);
  REQUIRE(NRef<Boolean>(jNode[2]).value() == true);
  REQUIRE(NRef<Null>(jNode[3]).value() == nullptr);
}
/// <summary>
/// Verify that an Node Object has the correct parsed format.
/// </summary>
/// <param name="jNode">Pointer to Node Object</param>
/// <returns></returns>
void checkObject(const Node &jNode)
{// {"City":"Southampton","Population":500000}
  REQUIRE_FALSE(!isA<Object>(jNode));
  REQUIRE(NRef<Object>(jNode).size() == 2);
  REQUIRE(NRef<Object>(jNode).contains("City"));
  REQUIRE(NRef<Object>(jNode).contains("Population"));
  REQUIRE_FALSE(!isA<String>(jNode["City"]));
  REQUIRE_FALSE(!isA<Number>(jNode["Population"]));
  REQUIRE(NRef<String>(jNode["City"]).value() == "Southampton");
  REQUIRE(NRef<Number>(jNode["Population"]).value<int>() == 500000);
}
/// <summary>
/// Strip white space from source JSON and place the remainder in destination.
/// </summary>
/// <param name="jsonBuffer">Source JSON</param>
/// <returns></returns>
std::string stripWhiteSpace(const std::string &jsonBuffer)
{
  const JSON json;
  BufferDestination destination;
  json.strip(BufferSource{ jsonBuffer }, destination);
  return destination.toString();
}
/// <summary>
/// Generate unique file name.
/// </summary>
/// <returns>Unique torrent file name</returns>
std::string generateRandomFileName(void) {
  std::filesystem::path namepath = std::tmpnam(nullptr);
  std::string result { std::filesystem::temp_directory_path().string() };
  result.push_back(std::filesystem::path::preferred_separator);
  return result+namepath.filename().string();
}
std::string generateEscapes(const unsigned char first, const unsigned char last)
{
  std::string result;
  for (char16_t utf16Char = first; utf16Char < last + 1; utf16Char++) {
    const auto digits = "0123456789ABCDEF";
    result += "\\u";
    result += digits[utf16Char >> 12 & 0x0f];
    result += digits[utf16Char >> 8 & 0x0f];
    result += digits[utf16Char >> 4 & 0x0f];
    result += digits[utf16Char&0x0f];
  }
  return result;
}