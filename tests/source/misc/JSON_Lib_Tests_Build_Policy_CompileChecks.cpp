#include "JSON_Lib_Tests.hpp"
#include <type_traits>

namespace {
  template<typename T>
  concept HasFromFile = requires(std::string_view path) {
    T::fromFile(path);
  };

  template<typename T>
  concept HasToFile = requires(std::string_view path, std::string_view json) {
    T::toFile(path, json);
  };

  template<typename T>
  concept HasGetFileFormat = requires(std::string_view path) {
    T::getFileFormat(path);
  };
}

TEST_CASE("Build policy compile-time detection for no-stdio and file APIs", "[JSON][Build][NoStdio][CompileTime]")
{
  REQUIRE(EmbeddedJSON::isNoStdIoBuild() == static_cast<bool>(JSON_LIB_NO_STDIO));

#if JSON_LIB_NO_STDIO
  static_assert(!HasFromFile<JSON>, "JSON::fromFile should be disabled when JSON_LIB_NO_STDIO is enabled.");
  static_assert(!HasToFile<JSON>, "JSON::toFile should be disabled when JSON_LIB_NO_STDIO is enabled.");
  static_assert(!HasGetFileFormat<JSON>, "JSON::getFileFormat should be disabled when JSON_LIB_NO_STDIO is enabled.");
  REQUIRE_FALSE(HasFromFile<JSON>);
  REQUIRE_FALSE(HasToFile<JSON>);
  REQUIRE_FALSE(HasGetFileFormat<JSON>);
#else
  static_assert(HasFromFile<JSON>, "JSON::fromFile should be available when JSON_LIB_NO_STDIO is disabled.");
  static_assert(HasToFile<JSON>, "JSON::toFile should be available when JSON_LIB_NO_STDIO is disabled.");
  static_assert(HasGetFileFormat<JSON>, "JSON::getFileFormat should be available when JSON_LIB_NO_STDIO is disabled.");
  REQUIRE(HasFromFile<JSON>);
  REQUIRE(HasToFile<JSON>);
  REQUIRE(HasGetFileFormat<JSON>);
#endif
}

TEST_CASE("Build policy compile-time detection for no-dynamic-memory and heap policy", "[JSON][Build][NoDynamicMemory][CompileTime]")
{
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild() == static_cast<bool>(JSON_LIB_NO_DYNAMIC_MEMORY));

#if JSON_LIB_NO_DYNAMIC_MEMORY
  static_assert(JSON_LIB_NO_HEAP, "JSON_LIB_NO_DYNAMIC_MEMORY builds must also disable heap allocation.");
  REQUIRE(JSON_LIB_NO_HEAP);
#else
  static_assert(!JSON_LIB_NO_HEAP || JSON_LIB_NO_DYNAMIC_MEMORY,
    "Heap allocation may only be disabled when no-dynamic-memory is enabled.");
#endif
}
