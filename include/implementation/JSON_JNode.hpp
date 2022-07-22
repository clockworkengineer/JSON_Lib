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
struct JNodeVariant;
struct JNode {
  // Pointer to JNode
  using Ptr = std::unique_ptr<JNode>;
  // Possible JSON node value types
  using InternalTypes =
      std::variant<int, long, long long, float, double, long double, bool,
                   std::string, std::nullptr_t>;
  // JNode Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  explicit JNode(std::unique_ptr<JNodeVariant> jNodeVariant)
      : m_jNodeVariant(std::move(jNodeVariant)) {}
  JNode(const std::initializer_list<InternalTypes> &array);
  JNode(
      const std::initializer_list<std::pair<std::string, InternalTypes>> &object);
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  JNode &operator=(float floatingPoint);
  JNode &operator=(double floatingPoint);
  JNode &operator=(long double floatingPoint);
  JNode &operator=(int integer);
  JNode &operator=(long integer);
  JNode &operator=(long long integer);
  JNode &operator=(const char *cString);
  JNode &operator=(const std::string &string);
  JNode &operator=(bool boolean);
  JNode &operator=(std::nullptr_t null);
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get JNode type
  [[nodiscard]] JNodeType getNodeType() const;
  // Get reference to JNodeVariant
  [[nodiscard]] std::unique_ptr<JNodeVariant> &getJNodeVariant();
  [[nodiscard]] const std::unique_ptr<JNodeVariant> &getJNodeVariant() const;

private:
  std::unique_ptr<JNodeVariant> m_jNodeVariant;
};
} // namespace JSONLib