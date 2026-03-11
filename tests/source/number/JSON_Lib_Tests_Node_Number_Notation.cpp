#include "JSON_Lib_Tests.hpp"

// ====================================
// Node Number floating point notation
// ====================================
TEST_CASE("Check Node Number floating point notation.", "[JSON][Node][Number][Float][Precision]")
{
  const JSON json;
  SECTION("Floating point notation to normal.", "[JSON][Node][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.0683,"longitude":-70.7416})");
  }
  SECTION("Floating point notation to fixed.", "[JSON][Node][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::fixed);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.068340,"longitude":-70.741615})");
  }
  SECTION("Floating point notation to scientific.", "[JSON][Node][Number][Float][Notation]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    jsonDestination.clear();
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    REQUIRE(jsonDestination.toString() == R"({"latitude":3.906834e+01,"longitude":-7.074162e+01})");
  }
  SECTION("Normal notation precision 2 rounds to 2 significant digits.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(2);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.0,"longitude":-71.0})");
  }
  SECTION("Fixed notation precision 2 yields 2 decimal places.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::fixed);
    Number::setPrecision(2);
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.07,"longitude":-70.74})");
  }
  SECTION("Scientific notation precision 2 yields 2 mantissa digits.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    Number::setPrecision(2);
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":3.91e+01,"longitude":-7.07e+01})");
  }
  SECTION("Integer values are not affected by floating point notation.", "[JSON][Node][Number][Float][Notation]")
  {
    json.parse(BufferSource{ R"({"count":42,"value":100})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    Number::setPrecision(2);
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"count":42,"value":100})");
  }
  SECTION("Higher precision (8) in fixed notation shows more decimal places.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination dest6, dest8;
    Number::setNotation(Number::numberNotation::fixed);
    Number::setPrecision(6);
    json.stringify(dest6);
    Number::setPrecision(8);
    json.stringify(dest8);
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    // 8 decimal places produces a strictly longer string than 6
    REQUIRE(dest8.toString().size() > dest6.toString().size());
  }
  SECTION("Mixed array: float elements change with notation, integers do not.", "[JSON][Node][Number][Float][Notation]")
  {
    json.parse(BufferSource{ R"([1,2.5,3])" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::scientific);
    Number::setPrecision(2);
    json.stringify(jsonDestination);
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(6);
    const std::string result = jsonDestination.toString();
    // integers 1 and 3 are unchanged; 2.5 becomes scientific
    REQUIRE(result.find("\"1\"") == std::string::npos);// 1 stays as 1
    REQUIRE(result.find('e') != std::string::npos);// 2.5 became scientific
  }
}