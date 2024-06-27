#pragma once

namespace JSON_Lib {

struct Null : Variant
{
  // Constructors/Destructors
  Null() : Variant(Type::null) {}
  Null(const Null &other) = default;
  Null &operator=(const Null &other) = default;
  Null(Null &&other) = default;
  Null &operator=(Null &&other) = default;
  ~Null() = default;
  // Return null value
  [[nodiscard]] static void *value() { return nullptr; }
  // Return string representation of value
  [[nodiscard]] static std::string toString() { return "null"; }
};
}// namespace JSON_Lib