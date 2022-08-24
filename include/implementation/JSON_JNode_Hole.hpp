#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ====
// Hole
// ====
struct Hole : Variant
{
  // Constructors/Destructors
  Hole() : Variant(JNodeType::hole) {}
  Hole(const Hole &other) = delete;
  Hole &operator=(const Hole &other) = delete;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
  [[nodiscard]] std::string toString() const { return ("null"); }
  // Make Hole JNode
  static JNode make() { return (JNode{ std::make_unique<Hole>(Hole()) }); }
};
}// namespace JSONLib