#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check Node Number assignment/creation.", "[JSON][Node][Number][Assignment]")
{
  SECTION("Create Node with int assigment.", "[JSON][Node][Number][Integer][Assignment]")
  {
    Node jNode;
    jNode = 666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<int>());
    REQUIRE(JRef<Number>(jNode).value<int>() == 666);
  }
  SECTION("Create Node with long assigment.", "[JSON][Node][Number][Long][Assignment]")
  {
    Node jNode;
    jNode = 666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long>());
    REQUIRE(JRef<Number>(jNode).value<long>() == 666l);
  }
  SECTION("Create Node with long long assigment.", "[JSON][Node][Number][Long Long][Assignment]")
  {
    Node jNode;
    jNode = 666ll;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long long>());
    REQUIRE(JRef<Number>(jNode).value<long>() == 666ll);
  }
  SECTION("Create Node with float assigment.", "[JSON][Node][Number][Float][Assignment]")
  {
    Node jNode;
    jNode = 666.666f;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<float>(), 666.666f, 0.0001));
  }
  SECTION("Create Node with double assigment.", "[JSON][Node][Number][Double][Assignment]")
  {
    Node jNode;
    jNode = 666.666;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<double>(), 666.666, 0.0001));
  }
  SECTION("Create Node with long double assigment.", "[JSON][Node][Number][Long Double][Assignment]")
  {
    Node jNode;
    jNode = 666.666l;
    REQUIRE_FALSE(!JRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(JRef<Number>(jNode).value<long double>(), 666.666l, 0.0001));
  }
}
