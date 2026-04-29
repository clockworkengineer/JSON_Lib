#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace JSON_Lib {

struct Object;
struct Array;
struct Number;
struct String;
struct Boolean;
struct Null;
struct Hole;

struct Node
{
  using Storage = std::variant<std::monostate, std::unique_ptr<Object>, std::unique_ptr<Array>, Number, String, Boolean, Null, Hole>;

  // Node Error
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("Node Error: ").append(message)) {}
  };
  // Constructors/Destructors
  Node() = default;
  explicit Node(std::unique_ptr<Object> value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(std::unique_ptr<Array> value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(Number value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(String value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(Boolean value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(Null value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
  explicit Node(Hole value) : jNodeVariant(std::move(value)), jNodeEmpty(false) {}
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
  Node(Node &&other) noexcept : jNodeVariant(std::move(other.jNodeVariant)), jNodeEmpty(other.jNodeEmpty)
  {
    other.jNodeVariant = std::monostate{};
    other.jNodeEmpty = true;
  }
  Node &operator=(Node &&other) noexcept
  {
    if (this != &other) {
      jNodeVariant = std::move(other.jNodeVariant);
      jNodeEmpty = other.jNodeEmpty;
      other.jNodeVariant = std::monostate{};
      other.jNodeEmpty = true;
    }
    return *this;
  }
  ~Node() = default;
  // Assignment operators
  template<typename T> Node &operator=(T value) { return *this = Node(value); }
  // Has the variant been created
  [[nodiscard]] bool isEmpty() const { return jNodeEmpty || std::holds_alternative<std::monostate>(jNodeVariant); }
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Get reference to Node variant
  Variant &getVariant() { return getVariantImpl(jNodeVariant); }
  [[nodiscard]] const Variant &getVariant() const { return getVariantImpl(jNodeVariant); }
  // Make Node
  template<typename T, typename... Args> static auto make(Args &&...args)
  {
    if constexpr (std::is_same_v<T, Object>) {
      return Node{ std::make_unique<Object>(std::forward<Args>(args)...) };
    } else if constexpr (std::is_same_v<T, Array>) {
      return Node{ std::make_unique<Array>(std::forward<Args>(args)...) };
    } else {
      return Node{ T(std::forward<Args>(args)...) };
    }
  }

private:
  static Variant &getVariantImpl(Storage &storage);
  static const Variant &getVariantImpl(const Storage &storage);

  Storage jNodeVariant;
  bool jNodeEmpty{true};
};
}// namespace JSON_Lib