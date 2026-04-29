#pragma once

#include <string>
#include <string_view>

namespace JSON_Lib {

struct String
{
  constexpr static int64_t kDefMaxStringLength = 16*1024;
  // Constructors/Destructors
  String() = default;
  explicit String(const std::string_view &string) : jNodeString(string) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string_view value() { return jNodeString; }
  [[nodiscard]] std::string_view value() const { return jNodeString; }
  // Return string representation of value
  [[nodiscard]] const std::string &toString() const noexcept { return jNodeString; }
  [[nodiscard]] std::string_view toStringView() const noexcept { return jNodeString; }
  // Append and reserve helpers for parser string extraction
  void reserve(const std::size_t capacity) { jNodeString.reserve(capacity); }
  void append(char ch) { jNodeString.push_back(ch); }
  void append(const std::string_view &value) { jNodeString.append(value); }
  void pop_back() { jNodeString.pop_back(); }
  void clear() { jNodeString.clear(); }
  [[nodiscard]] std::size_t size() const noexcept { return jNodeString.size(); }
  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) { maxStringLength = length; }
  static uint64_t getMaxStringLength() { return maxStringLength; }

private:
  std::string jNodeString;
  inline static uint64_t maxStringLength { kDefMaxStringLength};
};
}// namespace JSON_Lib