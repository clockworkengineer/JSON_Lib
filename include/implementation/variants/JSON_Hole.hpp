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
// Hole
// ====
struct Hole : Variant
{
  // Constructors/Destructors
  Hole() : Variant(Variant::Type::hole) {}
  Hole(const Hole &other) = default;
  Hole &operator=(const Hole &other) = default;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return ("null"); }

};
}// namespace JSON_Lib