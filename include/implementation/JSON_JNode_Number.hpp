#pragma once
// =======
// C++ STL
// =======
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ======
// Number
// ======
struct Number : Variant
{
  // Constructors/Destructors
  Number() : Variant(JNode::Type::number) {}
  explicit Number(const Numeric &number) : Variant(JNode::Type::number), m_number(number) {}
  Number(const Number &other) = delete;
  Number &operator=(const Number &other) = delete;
  Number(Number &&other) = default;
  Number &operator=(Number &&other) = default;
  ~Number() = default;
  // Return reference to numberic
  [[nodiscard]] Numeric &getNumber() { return (m_number); }
  [[nodiscard]] const Numeric &getNumber() const { return (m_number); }
  // Convert number to string
  [[nodiscard]] std::string toString() const { return (m_number.getString()); }
  // Make Number JNode
  static JNode make(const Numeric &number) { return (JNode{ std::make_unique<Number>(Number{ number }) }); }

private:
  Numeric m_number{};
};
}// namespace JSONLib