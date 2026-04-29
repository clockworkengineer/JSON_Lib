#include "JSON_Lib_Tests.hpp"

namespace {

static std::string generateLargeArrayJSON(std::size_t count)
{
  std::string json;
  json.reserve(count * 6 + 2);
  json.push_back('[');
  for (std::size_t index = 0; index < count; ++index) {
    if (index != 0) { json.push_back(','); }
    json += std::to_string(index);
  }
  json.push_back(']');
  return json;
}

static std::string generateLargeObjectJSON(std::size_t count)
{
  std::string json;
  json.reserve(count * 16 + 2);
  json.push_back('{');
  for (std::size_t index = 0; index < count; ++index) {
    if (index != 0) { json.push_back(','); }
    json += "\"key";
    json += std::to_string(index);
    json += "\":";
    json += std::to_string(index);
  }
  json.push_back('}');
  return json;
}

}// namespace

TEST_CASE("Verify large JSON payload parse and stringify correctness.", "[JSON][Parse][Large][Stringify]")
{
  const auto largeArrayJson = generateLargeArrayJSON(5000);
  const auto largeObjectJson = generateLargeObjectJSON(1500);

  SECTION("Large numeric array round-trip remains identical")
  {
    JSON json;
    json.parse(BufferSource{ largeArrayJson });
    REQUIRE(json.root().is<JSON_Lib::Array>());
    REQUIRE(json.root().get<JSON_Lib::Array>().size() == 5000);

    BufferDestination destination;
    json.stringify(destination);
    REQUIRE(destination.toString() == largeArrayJson);

    JSON parsedAgain(destination.toString());
    REQUIRE(parsedAgain.root().is<JSON_Lib::Array>());
    REQUIRE(parsedAgain.root().get<JSON_Lib::Array>().size() == 5000);
    REQUIRE(parsedAgain.root().get<JSON_Lib::Array>()[1234].get<JSON_Lib::Number>().value<long long>() == 1234);
  }

  SECTION("Large object parse, lookup and stringify remain correct")
  {
    JSON json;
    json.parse(BufferSource{ largeObjectJson });
    REQUIRE(json.root().is<JSON_Lib::Object>());
    REQUIRE(json.root().get<JSON_Lib::Object>().size() == 1500);

    for (int lookupIndex = 0; lookupIndex < 20; ++lookupIndex) {
      const auto key = std::string("key") + std::to_string(lookupIndex * 10);
      const auto &entry = json.root()[key];
      REQUIRE_FALSE(entry.isEmpty());
      REQUIRE(entry.is<JSON_Lib::Number>());
      REQUIRE(entry.get<JSON_Lib::Number>().value<long long>() == static_cast<long long>(lookupIndex * 10));
    }

    BufferDestination destination;
    json.stringify(destination);
    REQUIRE(destination.toString() == largeObjectJson);
  }
}
