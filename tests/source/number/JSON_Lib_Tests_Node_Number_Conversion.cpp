#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check Node Number conversion.", "[JSON][Node][Number]")
{
  const JSON json;
  // Note: That floating point -> integer will be rounded up/down
  SECTION("Float converted to int.", "[JSON][Node][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<int>());
    REQUIRE(NRef<Number>(json.root()).value<int>() == 6788889);
  }
  SECTION("Float converted to long.", "[JSON][Node][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
    REQUIRE(NRef<Number>(json.root()).value<long>() == 6788889);
  }
  SECTION("Float converted to long long.", "[JSON][Node][Number][Float]")
  {
    BufferSource jsonSource{ "6788888.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    REQUIRE(NRef<Number>(json.root()).value<long long>() == 6788889ll);
  }
  SECTION("Float converted to double.", "[JSON][Node][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<double>(), 678.8990, 0.0001));
  }
  SECTION("Float converted to long double.", "[JSON][Node][Number][Float]")
  {
    BufferSource jsonSource{ "678.8990" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<long double>(), 678.8990l, 0.0001));
  }
  SECTION("Int converted to long.", "[JSON][Node][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
    REQUIRE(NRef<Number>(json.root()).value<long>() == 78989);
  }
  SECTION("Int converted to long long.", "[JSON][Node][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    REQUIRE(NRef<Number>(json.root()).value<long long>() == 78989ll);
  }
  SECTION("Int converted to float.", "[JSON][Node][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), 78989.0f, 0.0001));
  }
  SECTION("Int converted to double.", "[JSON][Node][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<double>(), 78989.0, 0.0001));
  }
  SECTION("Int converted to long double.", "[JSON][Node][Number][Int]")
  {
    BufferSource jsonSource{ "78989" };
    json.parse(jsonSource);
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<long double>(), 78989.0l, 0.0001));
  }
  SECTION("Large positive integer conversion.", "[JSON][Node][Number][Long Long]")
  {
    BufferSource jsonSource{ "9223372036854775807" };
    json.parse(jsonSource);
    REQUIRE_FALSE(NRef<Number>(json.root()).is<int>());
    // If long and long long same size then integer never stored in long long
    // but you can fetch it as a long long
    if constexpr (sizeof(long long) > sizeof(long)) {
      REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(!NRef<Number>(json.root()).is<long long>());
    } else {
      REQUIRE_FALSE(!NRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    }
    REQUIRE(NRef<Number>(json.root()).value<long long>() == 9223372036854775807ll);
  }
  SECTION("Large negative integer conversion.", "[JSON][Node][Number][Long Long]")
  {
    BufferSource jsonSource{ "-877994604561387500" };
    json.parse(jsonSource);
    REQUIRE_FALSE(NRef<Number>(json.root()).is<int>());
    // If long and long long same size then integer never stored in long long
    // but you can fetch it as a long long
    if constexpr (sizeof(long long) > sizeof(long)) {
      REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(!NRef<Number>(json.root()).is<long long>());
    } else {
      REQUIRE_FALSE(!NRef<Number>(json.root()).is<long>());
      REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    }
    REQUIRE(NRef<Number>(json.root()).value<long long>() == -877994604561387500ll);
  }
  SECTION("Positive integer overflow conversion.", "[JSON][Node][Number][Long Long]")
  {
    BufferSource jsonSource{ "9223372036854775808" };// LLONG_MAX + 1
    json.parse(jsonSource);
    REQUIRE_FALSE(NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), 9223372036854775808.0f, 0.0001));
  }
  SECTION("Negative integer overflow conversion.", "[JSON][Node][Number][Long Long]")
  {
    BufferSource jsonSource{ "-9223372036854775809" };// LLONG_MIN - 1
    json.parse(jsonSource);
    REQUIRE_FALSE(NRef<Number>(json.root()).is<int>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long>());
    REQUIRE_FALSE(NRef<Number>(json.root()).is<long long>());
    REQUIRE_FALSE(!NRef<Number>(json.root()).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(json.root()).value<float>(), -9223372036854775808.0f, 0.0001));
  }
}