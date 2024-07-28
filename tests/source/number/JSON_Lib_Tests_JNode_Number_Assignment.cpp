#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check JNode Number assignment/creation.", "[JSON][JNode][Number][Assignment]")
{
  SECTION("Create JNode with int assigment.", "[JSON][JNode][Number][Integer][Assignment]")
  {
    JNode jNode;
    jNode = 666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 666);
  }
  SECTION("Create JNode with long assigment.", "[JSON][JNode][Number][Long][Assignment]")
  {
    JNode jNode;
    jNode = 666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<long>() == 666l);
  }
  SECTION("Create JNode with long long assigment.", "[JSON][JNode][Number][Long Long][Assignment]")
  {
    JNode jNode;
    jNode = 666ll;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long>() == 666ll);
  }
  SECTION("Create JNode with float assigment.", "[JSON][JNode][Number][Float][Assignment]")
  {
    JNode jNode;
    jNode = 666.666f;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 666.666f, 0.0001));
  }
  SECTION("Create JNode with double assigment.", "[JSON][JNode][Number][Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 666.666, 0.0001));
  }
  SECTION("Create JNode with long double assigment.", "[JSON][JNode][Number][Long Double][Assignment]")
  {
    JNode jNode;
    jNode = 666.666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 666.666l, 0.0001));
  }
}
