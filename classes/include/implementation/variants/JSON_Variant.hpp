#pragma once

namespace JSON_Lib {

struct Variant
{
  // Node Variant Types
  enum class Type : uint8_t { base = 0, object, array, number, string, boolean, null, hole };
  // Constructors/Destructors
  explicit Variant(const Type nodeType = Type::base) : jNodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Get BNode type
  [[nodiscard]] Type getNodeType() const {
    return jNodeType;
  }

private:
  Type jNodeType;
};
}// namespace JSON_Lib