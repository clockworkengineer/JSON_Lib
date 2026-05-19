#include "JSON_Lib_Tests.hpp"
#include "implementation/common/JSON_Attributes.hpp"

TEST_CASE("JSON_Attributes compiler detection macros are available", "[Compatibility][Portability]")
{
#if defined(_MSC_VER)
  REQUIRE(JSON_LIB_COMPILER_MSVC == 1);
#elif defined(__clang__)
  REQUIRE(JSON_LIB_COMPILER_CLANG == 1);
#elif defined(__GNUC__)
  REQUIRE(JSON_LIB_COMPILER_GCC == 1);
#else
  REQUIRE(JSON_LIB_COMPILER_UNKNOWN == 1);
#endif
}

TEST_CASE("JSON_Attributes platform detection macros are available", "[Compatibility][Portability]")
{
#if defined(_WIN32) || defined(_WIN64)
  REQUIRE(JSON_LIB_PLATFORM_WINDOWS == 1);
  REQUIRE(JSON_LIB_PLATFORM_UNIX == 0);
  REQUIRE(JSON_LIB_PLATFORM_POSIX == 0);
#else
  REQUIRE(JSON_LIB_PLATFORM_UNIX == 1);
  REQUIRE(JSON_LIB_PLATFORM_POSIX == 1);
  REQUIRE(JSON_LIB_PLATFORM_WINDOWS == 0);
#endif
}
