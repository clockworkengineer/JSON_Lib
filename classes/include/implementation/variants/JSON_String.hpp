#pragma once

namespace JSON_Lib {

struct String : Variant
{
  constexpr static int64_t kDefMaxStringLength = 16*1024;
  // Constructors/Destructors
  String() : Variant(Type::string) {}
  explicit String(std::string string) : Variant(Type::string), jNodeString(std::move(string)) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  [[nodiscard]] std::string &value() { return jNodeString; }
  [[nodiscard]] const std::string &value() const { return jNodeString; }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return jNodeString; }
  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) { maxStringLength = length; }
  static uint64_t getMaxStringLength() { return maxStringLength; }

private:
  std::string jNodeString;
  inline static uint64_t maxStringLength { kDefMaxStringLength};
};
}// namespace JSON_Lib