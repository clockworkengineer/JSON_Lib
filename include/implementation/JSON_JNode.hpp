#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =========================================================
// JNode forward declarations for interfaces/classes/structs
// =========================================================
#include "JSON_JNode_forward.hpp"
// =====
// JNode
// =====
struct JNode
{
  // JNode Type
  enum class Type { base = 0, object, array, number, string, boolean, null, hole };
  // JNode Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  JNode() = default;
  explicit JNode(float floatingPoint);
  explicit JNode(double floatingPoint);
  explicit JNode(int integer);
  explicit JNode(long integer);
  explicit JNode(const char *cString);
  explicit JNode(const std::string &string);
  explicit JNode(bool boolean);
  explicit JNode(std::nullptr_t null);
  JNode(const std::initializer_list<JSON::Types> &array);
  JNode(const std::initializer_list<std::pair<std::string, JSON::Types>> &object);
  explicit JNode(std::unique_ptr<Variant> jNodeVariant) : m_jNodeVariant(std::move(jNodeVariant)) {}
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  JNode &operator=(float floatingPoint);
  JNode &operator=(double floatingPoint);
  JNode &operator=(int integer);
  JNode &operator=(long integer);
  JNode &operator=(const char *cString);
  JNode &operator=(const std::string &string);
  JNode &operator=(bool boolean);
  JNode &operator=(std::nullptr_t null);
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
  JNode getJNode(const JSON::Types &type);
  std::unique_ptr<Variant> m_jNodeVariant;
};
}// namespace JSONLib