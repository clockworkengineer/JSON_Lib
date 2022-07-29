#pragma once
// ==========================
// C++ STL/ platform specific
// ==========================
#if defined(_WIN64)
#include "Windows.h"
#else
#include <codecvt>
#include <locale>
#endif
// ===================
// Converter interface
// ===================
#include "IConverter.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ================
// CLASS DEFINITION
// ================
class JSON_Converter : public IConverter {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  // ==============
  // PUBLIC METHODS
  // ==============
  [[nodiscard]] std::u16string toUtf16(const std::string &utf8) const override;
  [[nodiscard]] std::string toUtf8(const std::u16string &utf16) const override;
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  // =================
  // PRIVATE VARIABLES
  // =================
#if !defined(_WIN64)
  inline static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,
                                     char16_t>
      m_UTF16;
#endif
};
// ============================================================
// Windows API for converting between byte and wide characters.
// ============================================================
#if defined(_WIN64)
inline int WideCharToBytes(wchar_t *wideString, int wideStringLength,
                           char *bytes = nullptr, int length = 0) {
  return (WideCharToMultiByte(CP_UTF8, 0, wideString, wideStringLength, bytes,
                              length, nullptr, nullptr));
}
inline int BytesToWideChar(const char *bytes, int length,
                           wchar_t *sideString = nullptr,
                           int wideStringLength = 0) {
  return (MultiByteToWideChar(CP_UTF8, 0, bytes, length, sideString,
                              wideStringLength));
}
#endif
} // namespace JSONLib
