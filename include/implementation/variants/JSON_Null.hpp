#pragma once

#include <string>
namespace JSON_Lib {
// ====
// Null
// ====
struct Null : Variant
{
  // Constructors/Destructors
  Null() : Variant(Variant::Type::null) {}
  Null(const Null &other) = default;
  Null &operator=(const Null &other) = default;
  Null(Null &&other) = default;
  Null &operator=(Null &&other) = default;
  ~Null() = default;
  // Return null value
  [[nodiscard]] void *getNull() const { return nullptr; }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return "null"; }
};
}// namespace JSON_Lib