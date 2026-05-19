#pragma once

#ifndef __has_cpp_attribute
  #define __has_cpp_attribute(x) 0
#endif

#if defined(_MSC_VER)
  #define JSON_LIB_COMPILER_MSVC 1
#else
  #define JSON_LIB_COMPILER_MSVC 0
#endif

#if defined(__clang__)
  #define JSON_LIB_COMPILER_CLANG 1
#else
  #define JSON_LIB_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #define JSON_LIB_COMPILER_GCC 1
#else
  #define JSON_LIB_COMPILER_GCC 0
#endif

#if !JSON_LIB_COMPILER_MSVC && !JSON_LIB_COMPILER_CLANG && !JSON_LIB_COMPILER_GCC
  #define JSON_LIB_COMPILER_UNKNOWN 1
#else
  #define JSON_LIB_COMPILER_UNKNOWN 0
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define JSON_LIB_PLATFORM_WINDOWS 1
#else
  #define JSON_LIB_PLATFORM_WINDOWS 0
#endif

#if defined(__unix__) || defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__MACH__)
  #define JSON_LIB_PLATFORM_UNIX 1
#else
  #define JSON_LIB_PLATFORM_UNIX 0
#endif

#if JSON_LIB_PLATFORM_UNIX
  #define JSON_LIB_PLATFORM_POSIX 1
#else
  #define JSON_LIB_PLATFORM_POSIX 0
#endif

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
