#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace JSON_Lib {

struct Node
{
  explicit Node(std::unique_ptr<Variant> variant) : jNodeVariant(std::move(variant)) {}

  // Node Error
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("Node Error: ").append(message)) {}
  };
  // Constructors/Destructors
  Node() = default;
  template<typename T, typename = std::enable_if_t<
      std::is_same_v<T, bool> ||
      std::is_arithmetic_v<T> ||
      std::is_same_v<T, std::nullptr_t> ||
      std::is_convertible_v<T, std::string_view>>>
  explicit Node(T value);
  Node(const JSON::ArrayInitializer &array);
  Node(const JSON::ObjectInitializer &object);
  Node(const Node &other) = delete;
  Node &operator=(const Node &other) = delete;
  Node(Node &&other) = default;
  Node &operator=(Node &&other) = default;
  ~Node() = default;
  // Assignment operators
  template<typename T> Node &operator=(T value) { return *this = Node(value); }
  // Has the variant been created
  [[nodiscard]] bool isEmpty() const { return jNodeVariant == nullptr; }
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Get reference to Node variant
  Variant &getVariant() { return *jNodeVariant; }
  [[nodiscard]] const Variant &getVariant() const { return *jNodeVariant; }
  // Make Node
  template<typename T, typename... Args> static auto make(Args &&...args)
  {
    return Node{ std::make_unique<T>(std::forward<Args>(args)...) };
  }

private:
  std::unique_ptr<Variant> jNodeVariant;
};
}// namespace JSON_Lib