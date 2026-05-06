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
  #if __has_cpp_attribute(maybe_unused)
    #define JSON_LIB_MAYBE_UNUSED [[maybe_unused]]
  #else
    #define JSON_LIB_MAYBE_UNUSED
  #endif
  #if __has_cpp_attribute(likely)
    #define JSON_LIB_LIKELY [[likely]]
  #else
    #define JSON_LIB_LIKELY
  #endif
  #if __has_cpp_attribute(unlikely)
    #define JSON_LIB_UNLIKELY [[unlikely]]
  #else
    #define JSON_LIB_UNLIKELY
  #endif
#else
  #define JSON_LIB_NODISCARD
  #define JSON_LIB_DEPRECATED(message)
  #define JSON_LIB_FALLTHROUGH
  #define JSON_LIB_MAYBE_UNUSED
  #define JSON_LIB_LIKELY
  #define JSON_LIB_UNLIKELY
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
  #if defined(JSON_LIB_BUILDING_DLL)
    #define JSON_LIB_API JSON_LIB_EXPORT
  #else
    #define JSON_LIB_API JSON_LIB_IMPORT
  #endif
#else
  #define JSON_LIB_API
#endif
