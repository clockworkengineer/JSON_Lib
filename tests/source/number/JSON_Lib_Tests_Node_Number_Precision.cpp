#include "JSON_Lib_Tests.hpp"

// =====================================
// Node Number floating point precision
// =====================================
TEST_CASE("Check Node Number floating point precision.", "[JSON][Node][Number][Float][Precision]")
{
  const JSON json;
  SECTION("Floating point precision to 6 (default).", "[JSON][Node][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(6);
    json.stringify(jsonDestination);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.0683,"longitude":-70.7416})");
  }
  SECTION("Floating point precision to 7.", "[JSON][Node][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(7);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.06834,"longitude":-70.74162})");
  }
  SECTION("Floating point precision to 8.", "[JSON][Node][Number][Float][Precision]")
  {
    std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
    BufferSource jsonSource{ expected };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
    REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
    BufferDestination jsonDestination;
    Number::setPrecision(8);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.06834,"longitude":-70.741615})");
  }
  //   SECTION("Floating point precision to maximum.", "[JSON][Node][Number][Float][Precision]")
  //   {
  //     std::string expected{ R"({"latitude":39.068341,"longitude":-70.741615})" };
  //     BufferSource jsonSource{ expected };
  //     json.parse(jsonSource);
  //     REQUIRE_FALSE(!NRef<Number>(json["latitude"]).is<float>());
  //     REQUIRE_FALSE(!NRef<Number>(json["longitude"]).is<float>());
  //     BufferDestination jsonDestination;
  //     Number::setPrecision(std::numeric_limits<long double>::digits10 + 1);
  //     json.stringify(jsonDestination);
  //     Number::setPrecision(6);
  //     if constexpr (std::numeric_limits<long double>::digits10 + 1 == 16) {
  //       REQUIRE(jsonDestination.toString() == R"({"latitude":39.06834030151367,"longitude":-70.74161529541016})");
  //     } else if constexpr (std::numeric_limits<long double>::digits10 + 1 == 19) {
  //       REQUIRE(jsonDestination.toString() ==
  //       R"({"latitude":39.06834030151367188,"longitude":-70.74161529541015625})");
  //     } else {
  //       REQUIRE_FALSE(true);
  //     }
  //   }
  SECTION("Precision 1 yields single significant digit.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(1);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":4e+01.0,"longitude":-7e+01.0})");
  }
  SECTION("Precision 3 yields three significant digits.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(3);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"latitude":39.1,"longitude":-70.7})");
  }
  SECTION("Increasing precision produces longer or equal output string.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"latitude":39.068341,"longitude":-70.741615})" });
    Number::setNotation(Number::numberNotation::normal);
    BufferDestination dest4, dest8;
    Number::setPrecision(4);
    json.stringify(dest4);
    Number::setPrecision(8);
    json.stringify(dest8);
    Number::setPrecision(6);
    REQUIRE(dest8.toString().size() >= dest4.toString().size());
  }
  SECTION("Precision affects double values as well as float.", "[JSON][Node][Number][Float][Precision]")
  {
    // Parse a value that requires double precision to store exactly
    json.parse(BufferSource{ R"({"pi":3.141592653589793})" });
    Number::setNotation(Number::numberNotation::normal);
    BufferDestination dest4, dest10;
    Number::setPrecision(4);
    json.stringify(dest4);
    Number::setPrecision(10);
    json.stringify(dest10);
    Number::setPrecision(6);
    // Higher precision should represent more digits of pi
    REQUIRE(dest10.toString().size() > dest4.toString().size());
  }
  SECTION("Integer values are unaffected by precision setting.", "[JSON][Node][Number][Float][Precision]")
  {
    json.parse(BufferSource{ R"({"count":42})" });
    BufferDestination jsonDestination;
    Number::setNotation(Number::numberNotation::normal);
    Number::setPrecision(1);
    json.stringify(jsonDestination);
    Number::setPrecision(6);
    REQUIRE(jsonDestination.toString() == R"({"count":42})");
  }
}