#include "JSON_Lib_Tests.hpp"

TEST_CASE("Check Node Number assignment/creation.", "[JSON][Node][Number][Assignment]")
{
  SECTION("Create Node with int assigment.", "[JSON][Node][Number][Integer][Assignment]")
  {
    Node jNode;
    jNode = 666;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<int>());
    REQUIRE(NRef<Number>(jNode).value<int>() == 666);
  }
  SECTION("Create Node with long assigment.", "[JSON][Node][Number][Long][Assignment]")
  {
    Node jNode;
    jNode = 666l;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<long>());
    REQUIRE(NRef<Number>(jNode).value<long>() == 666l);
  }
  SECTION("Create Node with long long assigment.", "[JSON][Node][Number][Long Long][Assignment]")
  {
    Node jNode;
    jNode = 666ll;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<long long>());
    REQUIRE(NRef<Number>(jNode).value<long>() == 666ll);
  }
  SECTION("Create Node with float assigment.", "[JSON][Node][Number][Float][Assignment]")
  {
    Node jNode;
    jNode = 666.666f;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<float>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<float>(), 666.666f, 0.0001));
  }
  SECTION("Create Node with double assigment.", "[JSON][Node][Number][Double][Assignment]")
  {
    Node jNode;
    jNode = 666.666;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<double>(), 666.666, 0.0001));
  }
  SECTION("Create Node with long double assigment.", "[JSON][Node][Number][Long Double][Assignment]")
  {
    Node jNode;
    jNode = 666.666l;
    REQUIRE_FALSE(!NRef<Number>(jNode).is<long double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<long double>(), 666.666l, 0.0001));
  }
  SECTION("Assign zero integer to Node.", "[JSON][Node][Number][Integer][Assignment]")
  {
    Node jNode;
    jNode = 0;
    REQUIRE(NRef<Number>(jNode).is<int>());
    REQUIRE(NRef<Number>(jNode).value<int>() == 0);
  }
  SECTION("Assign negative integer to Node.", "[JSON][Node][Number][Integer][Assignment]")
  {
    Node jNode;
    jNode = -42;
    REQUIRE(NRef<Number>(jNode).is<int>());
    REQUIRE(NRef<Number>(jNode).value<int>() == -42);
  }
  SECTION("Assign negative double to Node.", "[JSON][Node][Number][Double][Assignment]")
  {
    Node jNode;
    jNode = -3.14;
    REQUIRE(NRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<double>(), -3.14, 0.0001));
  }
  SECTION("is<int>() returns false for a double node.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 1.5;
    REQUIRE_FALSE(NRef<Number>(jNode).is<int>());
    REQUIRE(NRef<Number>(jNode).is<double>());
  }
  SECTION("is<double>() returns false for an int node.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 99;
    REQUIRE_FALSE(NRef<Number>(jNode).is<double>());
    REQUIRE(NRef<Number>(jNode).is<int>());
  }
  SECTION("Cross-type value<double>() read from int node.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 10;
    REQUIRE(NRef<Number>(jNode).is<int>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<double>(), 10.0, 0.0001));
  }
  SECTION("Number::toString() on integer node.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 42;
    REQUIRE(NRef<Number>(jNode).toString() == "42");
  }
  SECTION("Number::toString() on negative integer node.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = -7;
    REQUIRE(NRef<Number>(jNode).toString() == "-7");
  }
  SECTION("Number::set<int>() changes node value.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 100;
    NRef<Number>(jNode).set(200);
    REQUIRE(NRef<Number>(jNode).is<int>());
    REQUIRE(NRef<Number>(jNode).value<int>() == 200);
  }
  SECTION("Number::set<double>() changes type from int to double.", "[JSON][Node][Number][Assignment]")
  {
    Node jNode;
    jNode = 1;
    REQUIRE(NRef<Number>(jNode).is<int>());
    NRef<Number>(jNode).set(3.14);
    REQUIRE(NRef<Number>(jNode).is<double>());
    REQUIRE_FALSE(!equalFloatingPoint(NRef<Number>(jNode).value<double>(), 3.14, 0.0001));
  }
}
