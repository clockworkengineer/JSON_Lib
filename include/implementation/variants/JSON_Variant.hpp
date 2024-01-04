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
  // Interrogate variant
  bool isObject() const { return (jNodeType == Variant::Type::object); }
  bool isArray() const { return (jNodeType == Variant::Type::array); }
  bool isNumber() const { return (jNodeType == Variant::Type::number); }
  bool isString() const { return (jNodeType == Variant::Type::string); }
  bool isBoolean() const { return (jNodeType == Variant::Type::boolean); }
  bool isNull() const { return (jNodeType == Variant::Type::null); }
  bool isHole() const { return (jNodeType == Variant::Type::hole); }

private:
  Variant::Type jNodeType;
};
}// namespace JSON_Lib