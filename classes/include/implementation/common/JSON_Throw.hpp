#pragma once

#include "JSON_Config.hpp"
#include <cstdlib>   // std::abort

// ============================================================
// JSON_THROW(exception) — unified exception dispatch macro.
//
// Default build (JSON_LIB_NO_EXCEPTIONS == 0):
//   Expands to the standard `throw exception` expression.
//
// No-exceptions build (JSON_LIB_NO_EXCEPTIONS == 1):
//   Expands to std::abort() which is [[noreturn]], satisfying
//   the compiler's control-flow analysis for both void and
//   non-void return contexts without any cast or workaround.
//   This allows the library to link with -fno-exceptions.
// ============================================================

#if JSON_LIB_NO_EXCEPTIONS
  #define JSON_THROW(exception) (std::abort())
#else
  #define JSON_THROW(exception) throw exception
#endif
