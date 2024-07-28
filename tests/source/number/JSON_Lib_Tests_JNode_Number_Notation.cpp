#include "JSON_Lib_Tests.hpp"

// ====================================
// JNode Number floating point notation
// ====================================
TEST_CASE("Check JNode Number floating point notation.", "[JSON][JNode][Number][Float][Precision]")
{
  const JSON json;
  SECTION("Floating point notation to normal.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.0683,"longitude":-70.7416})");
  }
  SECTION("Floating point notation to fixed.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::fixed);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.068340,"longitude":-70.741615})");
  }
  SECTION("Floating point notation to scientific.", "[JSON][JNode][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!JRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!JRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.toString() == R"({"latitude":3.906834e+01,"longitude":-7.074162e+01})");
  }
}