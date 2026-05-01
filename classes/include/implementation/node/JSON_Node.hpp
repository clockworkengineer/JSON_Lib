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
  explicit Node(std::unique_ptr<Object> value) : jNodeVariant(std::move(value)) {}
  explicit Node(std::unique_ptr<Array> value) : jNodeVariant(std::move(value)) {}
  explicit Node(Number value) : jNodeVariant(std::move(value)) {}
  explicit Node(String value) : jNodeVariant(std::move(value)) {}
  explicit Node(Boolean value) : jNodeVariant(std::move(value)) {}
  explicit Node(Null value) : jNodeVariant(std::move(value)) {}
  explicit Node(Hole value) : jNodeVariant(std::move(value)) {}
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
  Node(Node &&other) noexcept : jNodeVariant(std::move(other.jNodeVariant))
  {
    other.jNodeVariant = std::monostate{};
  }
  Node &operator=(Node &&other) noexcept
  {
    if (this != &other) {
      jNodeVariant = std::move(other.jNodeVariant);
      other.jNodeVariant = std::monostate{};
    }
    return *this;
  }
  ~Node() = default;
  // Assignment operators
  template<typename T> Node &operator=(T value) { return *this = Node(value); }
  // Has the variant been created
  [[nodiscard]] bool isEmpty() const { return std::holds_alternative<std::monostate>(jNodeVariant); }
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  template<typename T> using BaseType = std::remove_cv_t<T>;
  template<typename T> using StorageType = std::conditional_t<
      std::is_same_v<BaseType<T>, Object>, std::unique_ptr<Object>,
      std::conditional_t<std::is_same_v<BaseType<T>, Array>, std::unique_ptr<Array>, BaseType<T>>>;

  template<typename T> [[nodiscard]] bool is() const
  {
    return std::holds_alternative<StorageType<T>>(jNodeVariant);
  }

  template<typename T> T &get()
  {
    if constexpr (std::is_same_v<BaseType<T>, Object> || std::is_same_v<BaseType<T>, Array>) {
      return *std::get<StorageType<T>>(jNodeVariant);
    } else {
      return std::get<StorageType<T>>(jNodeVariant);
    }
  }

  template<typename T> const T &get() const
  {
    if constexpr (std::is_same_v<BaseType<T>, Object> || std::is_same_v<BaseType<T>, Array>) {
      return *std::get<StorageType<T>>(jNodeVariant);
    } else {
      return std::get<StorageType<T>>(jNodeVariant);
    }
  }
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
  Storage jNodeVariant;
};
}// namespace JSON_Lib