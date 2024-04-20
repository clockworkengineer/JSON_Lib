#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace JSON_Lib {
struct Error : public std::runtime_error
{
  explicit Error(const std::string &message) : std::runtime_error("JSON_Converter Error: " + message) {}
};
[[nodiscard]] std::u16string toUtf16(const std::string &utf8);
[[nodiscard]] std::string toUtf8(const std::u16string &utf16);
}// namespace JSON_Lib
