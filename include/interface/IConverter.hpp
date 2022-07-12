#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =======================================
// Interface for utf8 <-> uft16 conversion
// =======================================
class IConverter {
public:
  // ========================
  // Constructors/destructors
  // ========================
  IConverter() = default;
  IConverter(const IConverter &other) = delete;
  IConverter &operator=(const IConverter &other) = delete;
  IConverter(IConverter &&other) = delete;
  IConverter &operator=(IConverter &&other) = delete;
  virtual ~IConverter() = default;
  // ==================
  // From utf8 to utf16
  // ==================
  virtual std::u16string toUtf16(const std::string &utf8) = 0;
  // ==================
  // From utf16 to utf8
  // ==================
  virtual std::string toUtf8(const std::u16string &utf16) = 0;
};
} // namespace JSONLib