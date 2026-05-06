#pragma once

#if defined(__has_cpp_attribute)
  #if __has_cpp_attribute(nodiscard)
    #define JSON_LIB_NODISCARD [[nodiscard]]
  #else
    #define JSON_LIB_NODISCARD
  #endif
  #if __has_cpp_attribute(deprecated)
    #define JSON_LIB_DEPRECATED(message) [[deprecated(message)]]
  #else
    #define JSON_LIB_DEPRECATED(message)
  #endif
  #if __has_cpp_attribute(fallthrough)
    #define JSON_LIB_FALLTHROUGH [[fallthrough]]
  #else
    #define JSON_LIB_FALLTHROUGH
  #endif
#else
  #define JSON_LIB_NODISCARD
  #define JSON_LIB_DEPRECATED(message)
  #define JSON_LIB_FALLTHROUGH
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
  #define JSON_LIB_NOEXCEPT noexcept
  #define JSON_LIB_CONSTEXPR constexpr
#else
  #define JSON_LIB_NOEXCEPT
  #define JSON_LIB_CONSTEXPR
#endif

#if defined(_MSC_VER)
  #define JSON_LIB_EXPORT __declspec(dllexport)
  #define JSON_LIB_IMPORT __declspec(dllimport)
  #define JSON_LIB_HIDDEN
#elif defined(__GNUC__) || defined(__clang__)
  #define JSON_LIB_EXPORT __attribute__((visibility("default")))
  #define JSON_LIB_IMPORT
  #define JSON_LIB_HIDDEN __attribute__((visibility("hidden")))
#else
  #define JSON_LIB_EXPORT
  #define JSON_LIB_IMPORT
  #define JSON_LIB_HIDDEN
#endif

#if defined(JSON_LIB_SHARED)
  #define JSON_LIB_API JSON_LIB_EXPORT
#else
  #define JSON_LIB_API
#endif
