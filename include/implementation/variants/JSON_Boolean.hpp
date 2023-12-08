#pragma once

#include <string>

// =================
namespace JSON_Lib {
// =======
// Boolean
// =======
struct Boolean : Variant
{
  // Constructors/Destructors
  Boolean() : Variant(Variant::Type::boolean) {}
  explicit Boolean(bool boolean) : Variant(Variant::Type::boolean), jNodeBoolean(boolean) {}
  Boolean(const Boolean &other) = default;
  Boolean &operator=(const Boolean &other) = default;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return reference boolean value
  [[nodiscard]] bool &value() { return (jNodeBoolean); }
  [[nodiscard]] const bool &value() const { return (jNodeBoolean); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (jNodeBoolean ? "true" : "false"); }

private:
  bool jNodeBoolean{};
};
}// namespace JSON_Lib