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
// ===========
// JNode Types
// ===========
enum class JNodeType {
  base = 0,
  object,
  array,
  number,
  string,
  boolean,
  null,
  hole
};
// =====
// JNode
// =====
struct Variant;
struct JNode {
  // Possible JSON node value types
  using InternalTypes =
      std::variant<int, long, float, double, bool, std::string, std::nullptr_t>;
  // JNode Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("JNode Error: " + message) {}
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
  explicit JNode(std::unique_ptr<Variant> jNodeVariant)
      : m_jNodeVariant(std::move(jNodeVariant)) {}
  JNode(const std::initializer_list<InternalTypes> &array);
  JNode(const std::initializer_list<std::pair<std::string, InternalTypes>>
            &object);
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
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get JNode variant type
  [[nodiscard]] JNodeType getType() const;
  // Get reference to JNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant();
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const;

private:
  std::unique_ptr<Variant> m_jNodeVariant;
};
} // namespace JSONLib