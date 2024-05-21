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
  explicit Variant(Type nodeType = Type::base) : jNodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Interrogate variant
  [[nodiscard]] bool isObject() const { return jNodeType == Type::object; }
  [[nodiscard]] bool isArray() const { return jNodeType == Type::array; }
  [[nodiscard]] bool isNumber() const { return jNodeType == Type::number; }
  [[nodiscard]] bool isString() const { return jNodeType == Type::string; }
  [[nodiscard]] bool isBoolean() const { return jNodeType == Type::boolean; }
  [[nodiscard]] bool isNull() const { return jNodeType == Type::null; }
  [[nodiscard]] bool isHole() const { return jNodeType == Type::hole; }

private:
  Type jNodeType;
};
}// namespace JSON_Lib