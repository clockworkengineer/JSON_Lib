#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// =========================================================
// JNode forward declarations for interfaces/classes/structs
// =========================================================
#include "JSON_JNode_forward.hpp"
// =====
// JNode
// =====
struct JNode
{
  // JNode Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  JNode() = default;
  explicit JNode(float value);
  explicit JNode(double value);
  explicit JNode(long double value);
  explicit JNode(int value);
  explicit JNode(long value);
  explicit JNode(long long value);
  explicit JNode(const char *value);
  explicit JNode(const std::string &value);
  explicit JNode(bool value);
  explicit JNode(std::nullptr_t null);
  JNode(const std::initializer_list<JSON::InternalType> &array);
  JNode(const std::initializer_list<std::pair<std::string, JSON::InternalType>> &object);
  explicit JNode(std::unique_ptr<Variant> jNodeVariant) : m_variant(std::move(jNodeVariant)) {}
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  JNode &operator=(int value) { return (*this = JNode(value)); }
  JNode &operator=(long value) { return (*this = JNode(value)); }
  JNode &operator=(long long value) { return (*this = JNode(value)); }
  JNode &operator=(float value) { return (*this = JNode(value)); }
  JNode &operator=(double value) { return (*this = JNode(value)); }
  JNode &operator=(long double value) { return (*this = JNode(value)); }
  JNode &operator=(const char *value) { return (*this = JNode(value)); }
  JNode &operator=(const std::string &value) { return (*this = JNode(value)); }
  JNode &operator=(bool value) { return (*this = JNode(value)); }
  JNode &operator=([[maybe_unused]] std::nullptr_t value) { return (*this = JNode(value)); }
  // Interrogate variant
  [[nodiscard]] bool isEmpty() const;
  [[nodiscard]] bool isObject() const;
  [[nodiscard]] bool isArray() const;
  [[nodiscard]] bool isNumber() const;
  [[nodiscard]] bool isString() const;
  [[nodiscard]] bool isBoolean() const;
  [[nodiscard]] bool isNull() const;
  [[nodiscard]] bool isHole() const;
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get reference to JNode variant
  [[nodiscard]] Variant &getVariant();
  [[nodiscard]] const Variant &getVariant() const;

private:
  std::unique_ptr<Variant> m_variant;
};
}// namespace JSON_Lib