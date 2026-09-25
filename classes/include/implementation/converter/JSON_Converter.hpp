#pragma once

#include <string>
#include <string_view>

#if __has_include("JSON_Attributes.hpp")
#include "JSON_Attributes.hpp"
#else
#include "implementation/common/JSON_Attributes.hpp"
#endif

namespace JSON_Lib {
JSON_LIB_NODISCARD std::string toUtf8(char16_t utf16);
JSON_LIB_NODISCARD std::u16string toUtf16(const std::string_view &utf8);
JSON_LIB_NODISCARD std::string toUtf8(const std::u16string_view &utf16);
}// namespace JSON_Lib
