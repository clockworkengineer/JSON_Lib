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
  explicit Boolean(bool boolean) : Variant(Variant::Type::boolean), boolean(boolean) {}
  Boolean(const Boolean &other) = default;
  Boolean &operator=(const Boolean &other) = default;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return reference boolean value
  [[nodiscard]] bool &getBoolean() { return (boolean); }
  [[nodiscard]] const bool &getBoolean() const { return (boolean); }
  // Return string representation of value
  [[nodiscard]] std::string toString() const { return (boolean ? "true" : "false"); }

private:
  bool boolean{};
};
}// namespace JSON_Lib