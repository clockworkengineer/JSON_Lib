#pragma once
namespace JSON_Lib {
// ====
// Base
// ====
struct Variant
{
  // JNode Variant Types
  enum class Type : uint8_t { base = 0, object, array, number, string, boolean, null, hole };
  // Constructors/Destructors
  explicit Variant(Variant::Type nodeType = Variant::Type::base) : jNodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Get variant type
  [[nodiscard]] Variant::Type getType() const { return (jNodeType); }

private:
  Variant::Type jNodeType;
};
}// namespace JSON_Lib