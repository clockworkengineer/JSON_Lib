#pragma once

#include <string>
#include <string_view>
#include "JSON_Config.hpp"

namespace JSON_Lib {

extern inline uint64_t g_defaultStringLength;

struct String
{
#if JSON_LIB_MAX_STRING_LENGTH
  constexpr static int64_t kDefMaxStringLength = JSON_LIB_MAX_STRING_LENGTH;
#else
  constexpr static int64_t kDefMaxStringLength = 16*1024;
#endif
  // Constructors/Destructors
  String() : m_maxStringLength(g_defaultStringLength ? g_defaultStringLength : kDefMaxStringLength) {}
  explicit String(const std::string_view &string)
    : jNodeString(string), m_maxStringLength(g_defaultStringLength ? g_defaultStringLength : kDefMaxStringLength)
  {}
  String(const std::string_view &string, const uint64_t maxStringLength)
    : jNodeString(string), m_maxStringLength(maxStringLength)
  {}
  explicit String(const uint64_t maxStringLength)
    : m_maxStringLength(maxStringLength)
  {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  JSON_LIB_NODISCARD std::string_view value() { return jNodeString; }
  JSON_LIB_NODISCARD std::string_view value() const { return jNodeString; }
  // Return string representation of value
  JSON_LIB_NODISCARD const std::string &toString() const noexcept { return jNodeString; }
  JSON_LIB_NODISCARD std::string_view toStringView() const noexcept { return jNodeString; }
  // Append and reserve helpers for parser string extraction
  void reserve(const std::size_t capacity) { jNodeString.reserve(capacity); }
  void append(char ch) { jNodeString.push_back(ch); }
  void append(const std::string_view &value) { jNodeString.append(value); }
  void pop_back() { jNodeString.pop_back(); }
  void clear() { jNodeString.clear(); }
  JSON_LIB_NODISCARD std::size_t size() const noexcept { return jNodeString.size(); }
  // Set/get maximum string length
  void setMaxStringLength(const uint64_t length) { m_maxStringLength = length; }
  JSON_LIB_NODISCARD uint64_t getMaxStringLength() const noexcept { return m_maxStringLength; }

private:
  std::string jNodeString;
  uint64_t m_maxStringLength{ g_defaultStringLength ? g_defaultStringLength : kDefMaxStringLength };
};

inline uint64_t g_defaultStringLength = String::kDefMaxStringLength;

inline void setDefaultStringLength(const uint64_t length) { g_defaultStringLength = length; }
inline uint64_t getDefaultStringLength() noexcept { return g_defaultStringLength; }

}// namespace JSON_Lib