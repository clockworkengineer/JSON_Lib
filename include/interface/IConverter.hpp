#pragma once

#include <string>
#include <stdexcept>

namespace JSON_Lib {

// =======================================
// Interface for utf8 <-> uft16 conversion
// =======================================
class IConverter
{
public:
  // ================
  // IConverter Error
  // ================
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IConverter Error: " + message) {}
  };
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
  [[nodiscard]] virtual std::u16string toUtf16(const std::string &utf8) const = 0;
  // ==================
  // From utf16 to utf8
  // ==================
  [[nodiscard]] virtual std::string toUtf8(const std::u16string &utf16) const = 0;
};
}// namespace JSON_Lib