#pragma once

namespace JSON_Lib {
JSON_LIB_NODISCARD std::string toUtf8(char16_t utf16);
JSON_LIB_NODISCARD std::u16string toUtf16(const std::string_view &utf8);
JSON_LIB_NODISCARD std::string toUtf8(const std::u16string_view &utf16);
}// namespace JSON_Lib
