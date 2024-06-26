#pragma once

namespace JSON_Lib {

struct Hole : Variant
{
  // Constructors/Destructors
  Hole() : Variant(Type::hole) {}
  Hole(const Hole &other) = default;
  Hole &operator=(const Hole &other) = default;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
  // Return string representation of value
  [[nodiscard]] static std::string toString() { return "null"; }
};
}// namespace JSON_Lib