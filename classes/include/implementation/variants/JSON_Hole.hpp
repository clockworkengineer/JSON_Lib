#pragma once

namespace JSON_Lib {

struct Hole
{
  // Constructors/Destructors
  Hole() = default;
  Hole(const Hole &other) = default;
  Hole &operator=(const Hole &other) = default;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
  // Return string representation of value
  [[nodiscard]] static std::string toString() { return "null"; }
};
}// namespace JSON_Lib