#pragma once

#include <utility>

namespace JSON_Lib {

inline Node::Node() : jNodeVariant(std::monostate{}) {}
inline Node::Node(std::unique_ptr<Object> value) : jNodeVariant(std::move(value)) {}
inline Node::Node(std::unique_ptr<Array> value) : jNodeVariant(std::move(value)) {}
inline Node::Node(Number value) : jNodeVariant(std::move(value)) {}
inline Node::Node(String value) : jNodeVariant(std::move(value)) {}
inline Node::Node(Boolean value) : jNodeVariant(std::move(value)) {}
inline Node::Node(Null value) : jNodeVariant(std::move(value)) {}
inline Node::Node(Hole value) : jNodeVariant(std::move(value)) {}

inline Node::Node(Node &&other) noexcept : jNodeVariant(std::move(other.jNodeVariant))
{
  other.jNodeVariant = std::monostate{};
}

inline Node &Node::operator=(Node &&other) noexcept
{
  if (this != &other) {
    jNodeVariant = std::move(other.jNodeVariant);
    other.jNodeVariant = std::monostate{};
  }
  return *this;
}

inline Node::~Node() = default;

template<typename T>
inline Node &Node::operator=(T value)
{
  return *this = Node(value);
}

template<typename T, typename... Args>
Node Node::make(Args &&...args)
{
  if constexpr (std::is_same_v<T, Object>) {
    return Node{ std::make_unique<Object>(std::forward<Args>(args)...) };
  } else if constexpr (std::is_same_v<T, Array>) {
    return Node{ std::make_unique<Array>(std::forward<Args>(args)...) };
  } else {
    return Node{ T(std::forward<Args>(args)...) };
  }
}

// Construct Node from raw values
template<typename T, typename>
Node::Node(T value)
{
  if constexpr (std::is_same_v<T, bool>) {
    *this = Node::make<Boolean>(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    *this = Node::make<Number>(value);
  } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
    *this = make<Null>();
  } else if constexpr (std::is_convertible_v<T, std::string_view>) {
    *this = Node::make<String>(value);
  }
}

// Convert an initializer list type to Node
inline Node typeToNode(const JSON::InitializerListTypes &type)
{
  return std::visit(overloaded{
    [](int v)           -> Node { return Node(v); },
    [](long v)          -> Node { return Node(v); },
    [](long long v)     -> Node { return Node(v); },
    [](float v)         -> Node { return Node(v); },
    [](double v)        -> Node { return Node(v); },
    [](long double v)   -> Node { return Node(v); },
    [](bool v)          -> Node { return Node(v); },
    [](const std::string &v) -> Node { return Node(v); },
    [](std::nullptr_t)  -> Node { return Node(nullptr); },
    [](const Node &v)   -> Node { return std::move(*const_cast<Node *>(&v)); }
  }, type);
}

// Construct Node Array from initializer list
inline Node::Node(const JSON::ArrayInitializer &array)
{
  *this = make<Array>();
  for (const auto &entry : array) { NRef<Array>(*this).add(typeToNode(entry)); }
}

// Construct Node Object from initializer list
inline Node::Node(const JSON::ObjectInitializer &object)
{
  *this = make<Object>();
  for (const auto &[fst, snd] : object) { NRef<Object>(*this).add(Object::Entry(fst, typeToNode(snd))); }
}

}// namespace JSON_Lib
