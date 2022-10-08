#pragma once
// =======
// C++ STL
// =======
#include <string>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ====
// Null
// ====
struct Null : Variant
{
  // Constructors/Destructors
  Null() : Variant(Variant::Type::null) {}
  Null(const Null &other) = delete;
  Null &operator=(const Null &other) = delete;
  Null(Null &&other) = default;
  Null &operator=(Null &&other) = default;
  ~Null() = default;
  // Return null value
  [[nodiscard]] void *getNull() const { return (nullptr); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return ("null"); }
  // Make Null JNode
  static JNode make() { return (JNode{ std::make_unique<Null>(Null()) }); }
};
}// namespace JSON_Lib