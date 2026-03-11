#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JSON parsing of simple types (number,string,boolean,null).", "[JSON][Parse][Simple]")
{
  const JSON json;
  SECTION("Parse an string (example string).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<String>(json.root()));
  }
  SECTION("Parse an number (6767).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Number>(json.root()));
  }
  SECTION("Parse an boolean (true).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Boolean>(json.root()));
  }
  SECTION("Parse an boolean (false).", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Boolean>(json.root()));
  }
  SECTION("Parse an null.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<Null>(json.root()));
  }
  SECTION("Parse an empty string.", "[JSON][Parse][Simple]")
  {
    BufferSource jsonSource{ "\"\"" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!isA<String>(json.root()));
  }
}
TEST_CASE("Check JSON parsing of simple types and check values.", "[JSON][Parse][Simple][Validate]")
{
  const JSON json;
  SECTION("Parse an string (example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("example string")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "example string");
  }
  SECTION("Parse an string (another example string) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ R"("another example string")" };
    json.parse(jsonSource);
    REQUIRE(NRef<String>(json.root()).value() == "another example string");
  }
  SECTION("Parse an number (6767) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "6767" };
    json.parse(jsonSource);
    REQUIRE(NRef<Number>(json.root()).value<int>() == 6767);
  }
  SECTION("Parse an number (190000) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "190000" };
    json.parse(jsonSource);
    REQUIRE(NRef<Number>(json.root()).value<int>() == 190000);
  }
  SECTION("Parse an boolean (true) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "true" };
    json.parse(jsonSource);
    REQUIRE(NRef<Boolean>(json.root()).value() == true);
  }
  SECTION("Parse an boolean (false) and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "false" };
    json.parse(jsonSource);
    REQUIRE(NRef<Boolean>(json.root()).value() == false);
  }
  SECTION("Parse an null and check its value.", "[JSON][Parse][Simple][Validate]")
  {
    BufferSource jsonSource{ "null" };
    json.parse(jsonSource);
    REQUIRE(NRef<Null>(json.root()).value() == nullptr);
  }
}
TEST_CASE("Check JSON parsing of numbers (negative, float, large).", "[JSON][Parse][Simple][Number]")
{
  const JSON json;
  SECTION("Parse negative integer (-1024) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "-1024" });
    REQUIRE(NRef<Number>(json.root()).value<int>() == -1024);
  }
  SECTION("Parse zero (0) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "0" });
    REQUIRE(NRef<Number>(json.root()).value<int>() == 0);
  }
  SECTION("Parse a floating-point number (3.14159) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "3.14159" });
    REQUIRE(isA<Number>(json.root()));
    REQUIRE(equalFloatingPoint(NRef<Number>(json.root()).value<double>(), 3.14159, 1e-5));
  }
  SECTION("Parse a negative floating-point number (-2.718) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "-2.718" });
    REQUIRE(equalFloatingPoint(NRef<Number>(json.root()).value<double>(), -2.718, 1e-3));
  }
  SECTION("Parse a number in scientific notation (1e5) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "1e5" });
    REQUIRE(equalFloatingPoint(NRef<Number>(json.root()).value<double>(), 100000.0, 1e-3));
  }
  SECTION("Parse a large integer requiring long long (9000000000) and check value.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "9000000000" });
    REQUIRE(NRef<Number>(json.root()).value<long long>() == 9000000000LL);
  }
  SECTION("Parse integer and verify is<int>() returns true.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "42" });
    REQUIRE(NRef<Number>(json.root()).is<int>());
  }
  SECTION("Parse float and verify is<float>() or is<double>() returns true.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "1.5" });
    const auto &num = NRef<Number>(json.root());
    REQUIRE((num.is<float>() || num.is<double>() || num.is<long double>()));
  }
  SECTION("Parse integer and verify is<double>() returns false.", "[JSON][Parse][Simple][Number]")
  {
    json.parse(BufferSource{ "99" });
    REQUIRE_FALSE(NRef<Number>(json.root()).is<double>());
  }
}
TEST_CASE("Check JSON parsing of strings (edge cases).", "[JSON][Parse][Simple][String]")
{
  const JSON json;
  SECTION("Parse a string containing only spaces and check value.", "[JSON][Parse][Simple][String]")
  {
    json.parse(BufferSource{ R"("   ")" });
    REQUIRE(NRef<String>(json.root()).value() == "   ");
  }
  SECTION("Parse a string with numeric digits and check value.", "[JSON][Parse][Simple][String]")
  {
    json.parse(BufferSource{ R"("12345")" });
    REQUIRE(NRef<String>(json.root()).value() == "12345");
  }
  SECTION("Parse an empty string and check its value is empty.", "[JSON][Parse][Simple][String]")
  {
    json.parse(BufferSource{ "\"\"" });
    REQUIRE(NRef<String>(json.root()).value().empty());
  }
  SECTION("Parse a string with punctuation and check value.", "[JSON][Parse][Simple][String]")
  {
    json.parse(BufferSource{ R"("hello, world!")" });
    REQUIRE(NRef<String>(json.root()).value() == "hello, world!");
  }
}
TEST_CASE("Check JSON parsing resets correctly between parses.", "[JSON][Parse][Simple][Reset]")
{
  const JSON json;
  SECTION("Parse integer then string; root reflects latest parse.", "[JSON][Parse][Simple][Reset]")
  {
    BufferSource s1{ "42" };
    json.parse(s1);
    REQUIRE(isA<Number>(json.root()));
    BufferSource s2{ R"("hello")" };
    json.parse(s2);
    REQUIRE(isA<String>(json.root()));
  }
  SECTION("Parse true then false; root reflects latest parse.", "[JSON][Parse][Simple][Reset]")
  {
    BufferSource s1{ "true" };
    json.parse(s1);
    REQUIRE(NRef<Boolean>(json.root()).value() == true);
    BufferSource s2{ "false" };
    json.parse(s2);
    REQUIRE(NRef<Boolean>(json.root()).value() == false);
  }
}