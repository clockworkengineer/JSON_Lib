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
struct JNodeData;
struct JNode {
  // Pointer to JNode
  using Ptr = std::unique_ptr<JNode>;
  using InternalTypes =
      std::variant<int, long, long long, float, double, long double, bool,
                   std::string, std::nullptr_t>;
  // JNode Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  explicit JNode(std::unique_ptr<JNodeData> jNodeData)
      : m_jNodeData(std::move(jNodeData)) {}
  JNode(const std::initializer_list<InternalTypes> &list);
  JNode(
      const std::initializer_list<std::pair<std::string, InternalTypes>> &list);
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
  [[nodiscard]]JNodeType getNodeType() const;
  // Get reference to JNodeData
  [[nodiscard]]std::unique_ptr<JNodeData> &getJNodeData();
  [[nodiscard]]const std::unique_ptr<JNodeData> &getJNodeData() const;

private:
  std::unique_ptr<JNodeData> m_jNodeData;
};
} // namespace JSONLib