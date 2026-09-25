#pragma once

#include <memory>
#include <string_view>
#include "JSON_ErrorBase.hpp"
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
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("Node", message)) {}
  };
  // Constructors/Destructors
  Node();
  explicit Node(std::unique_ptr<Object> value);
  explicit Node(std::unique_ptr<Array> value);
  explicit Node(Number value);
  explicit Node(String value);
  explicit Node(Boolean value);
  explicit Node(Null value);
  explicit Node(Hole value);
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
  Node(Node &&other) noexcept;
  Node &operator=(Node &&other) noexcept;
  ~Node();
  // Assignment operators
  template<typename T> Node &operator=(T value);
  // Has the variant been created
  JSON_LIB_NODISCARD bool isEmpty() const { return std::holds_alternative<std::monostate>(jNodeVariant); }
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  void resize(std::size_t index);
  template<typename T> using BaseType = std::remove_cv_t<T>;
  template<typename T> using StorageType = std::conditional_t<
      std::is_same_v<BaseType<T>, Object>, std::unique_ptr<Object>,
      std::conditional_t<std::is_same_v<BaseType<T>, Array>, std::unique_ptr<Array>, BaseType<T>>>;

  template<typename T> JSON_LIB_NODISCARD bool is() const
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
  // Visit — calls vis with the concrete stored type (unwraps unique_ptr<Object/Array>)
  template<typename Visitor>
  auto visit(Visitor &&vis) const
  {
    return std::visit([&vis](const auto &v) -> decltype(auto) {
      using T = std::decay_t<decltype(v)>;
      if constexpr (std::is_same_v<T, std::unique_ptr<Object>>) {
        return vis(*v);
      } else if constexpr (std::is_same_v<T, std::unique_ptr<Array>>) {
        return vis(*v);
      } else {
        return vis(v);
      }
    }, jNodeVariant);
  }
  // Make Node
  template<typename T, typename... Args> static Node make(Args &&...args);

private:
  Storage jNodeVariant;
};

// Visitor helper — inherit from multiple lambdas
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

}// namespace JSON_Lib