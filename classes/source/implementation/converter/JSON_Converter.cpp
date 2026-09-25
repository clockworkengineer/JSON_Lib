//
// Class: Converter
//
// Description: Convert characters to/from UTF-8 and UTF-16.
//
// Standard C++20/C++23 implementation, fully thread-safe and portable.
//

#include "JSON.hpp"
#include "JSON_Converter.hpp"
#include "JSON_Error.hpp"
#include "JSON_Throw.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace JSON_Lib {

/// <summary>
/// Convert single UTF-16 code unit to UTF-8 string.
/// </summary>
std::string toUtf8(char16_t utf16)
{
  const std::u16string_view view(&utf16, 1);
  return toUtf8(view);
}

/// <summary>
/// Convert UTF-8 string to UTF-16 string.
/// </summary>
std::u16string toUtf16(const std::string_view &utf8)
{
  if (utf8.find('\0') != std::string_view::npos) {
    JSON_THROW(Error("Tried to convert a null character."));
  }

  std::u16string result;
  result.reserve(utf8.size());

  const auto *bytes = reinterpret_cast<const uint8_t *>(utf8.data());
  const size_t len = utf8.size();
  size_t i = 0;

  while (i < len) {
    const uint8_t b0 = bytes[i];

    if (b0 <= 0x7F) {
      result.push_back(static_cast<char16_t>(b0));
      ++i;
    } else if ((b0 & 0xE0) == 0xC0) {
      if (i + 1 >= len) {
        JSON_THROW(Error("Truncated UTF-8 sequence"));
      }
      const uint8_t b1 = bytes[i + 1];
      if ((b1 & 0xC0) != 0x80 || b0 < 0xC2) {
        JSON_THROW(Error("Invalid UTF-8 byte sequence"));
      }
      const uint32_t cp = ((b0 & 0x1F) << 6) | (b1 & 0x3F);
      result.push_back(static_cast<char16_t>(cp));
      i += 2;
    } else if ((b0 & 0xF0) == 0xE0) {
      if (i + 2 >= len) {
        JSON_THROW(Error("Truncated UTF-8 sequence"));
      }
      const uint8_t b1 = bytes[i + 1];
      const uint8_t b2 = bytes[i + 2];
      if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80) {
        JSON_THROW(Error("Invalid UTF-8 continuation bytes"));
      }
      if (b0 == 0xE0 && b1 < 0xA0) {
        JSON_THROW(Error("Overlong UTF-8 sequence"));
      }
      if (b0 == 0xED && b1 >= 0xA0) {
        JSON_THROW(Error("UTF-8 encoding of surrogate code point"));
      }
      const uint32_t cp = ((b0 & 0x0F) << 12) | ((b1 & 0x3F) << 6) | (b2 & 0x3F);
      result.push_back(static_cast<char16_t>(cp));
      i += 3;
    } else if ((b0 & 0xF8) == 0xF0) {
      if (i + 3 >= len) {
        JSON_THROW(Error("Truncated UTF-8 sequence"));
      }
      const uint8_t b1 = bytes[i + 1];
      const uint8_t b2 = bytes[i + 2];
      const uint8_t b3 = bytes[i + 3];
      if ((b1 & 0xC0) != 0x80 || (b2 & 0xC0) != 0x80 || (b3 & 0xC0) != 0x80) {
        JSON_THROW(Error("Invalid UTF-8 continuation bytes"));
      }
      if (b0 == 0xF0 && b1 < 0x90) {
        JSON_THROW(Error("Overlong UTF-8 sequence"));
      }
      if (b0 == 0xF4 && b1 > 0x8F) {
        JSON_THROW(Error("UTF-8 code point out of Unicode range"));
      }
      if (b0 > 0xF4) {
        JSON_THROW(Error("UTF-8 code point out of Unicode range"));
      }
      uint32_t cp = ((b0 & 0x07) << 18) | ((b1 & 0x3F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
      if (cp > 0x10FFFF) {
        JSON_THROW(Error("Unicode code point exceeds 0x10FFFF"));
      }
      // Encode as UTF-16 surrogate pair
      cp -= 0x10000;
      result.push_back(static_cast<char16_t>(0xD800 + (cp >> 10)));
      result.push_back(static_cast<char16_t>(0xDC00 + (cp & 0x3FF)));
      i += 4;
    } else {
      JSON_THROW(Error("Invalid leading UTF-8 byte"));
    }
  }

  return result;
}

/// <summary>
/// Convert UTF-16 string to UTF-8 string.
/// </summary>
std::string toUtf8(const std::u16string_view &utf16)
{
  if (utf16.find(u'\0') != std::u16string_view::npos) {
    JSON_THROW(Error("Tried to convert a null character."));
  }

  std::string result;
  result.reserve(utf16.size() * 3 / 2);

  const size_t len = utf16.size();
  size_t i = 0;

  while (i < len) {
    uint32_t cp = utf16[i];

    if (cp >= 0xD800 && cp <= 0xDBFF) {
      // High surrogate
      if (i + 1 >= len) {
        JSON_THROW(Error("Unpaired high surrogate in UTF-16 string"));
      }
      const uint32_t low = utf16[i + 1];
      if (low < 0xDC00 || low > 0xDFFF) {
        JSON_THROW(Error("Invalid low surrogate following high surrogate"));
      }
      cp = 0x10000 + ((cp - 0xD800) << 10) + (low - 0xDC00);
      ++i;
    } else if (cp >= 0xDC00 && cp <= 0xDFFF) {
      // Unpaired low surrogate
      JSON_THROW(Error("Unpaired low surrogate in UTF-16 string"));
    }

    if (cp <= 0x7F) {
      result.push_back(static_cast<char>(cp));
    } else if (cp <= 0x7FF) {
      result.push_back(static_cast<char>(0xC0 | (cp >> 6)));
      result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else if (cp <= 0xFFFF) {
      result.push_back(static_cast<char>(0xE0 | (cp >> 12)));
      result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
      result.push_back(static_cast<char>(0xF0 | (cp >> 18)));
      result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
      result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }

    ++i;
  }

  return result;
}

}// namespace JSON_Lib
