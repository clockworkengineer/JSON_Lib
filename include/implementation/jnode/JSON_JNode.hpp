#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

namespace JSON_Lib {

struct JNode
{
  // JNode Error
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  JNode() = default;
  template<typename T> explicit JNode(T value);
  JNode(const JSON::ArrayInitializer &array);
  JNode(const JSON::Objectintializer &object);
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  template<typename T> JNode &operator=(T value) { return *this = JNode(value); }
  // Interrogate variant
  [[nodiscard]] bool isEmpty() const { return jNodeVariant == nullptr; }
  [[nodiscard]] bool isObject() const { return jNodeVariant->isObject(); }
  [[nodiscard]] bool isArray() const { return jNodeVariant->isArray(); }
  [[nodiscard]] bool isNumber() const { return jNodeVariant->isNumber(); }
  [[nodiscard]] bool isString() const { return jNodeVariant->isString(); }
  [[nodiscard]] bool isBoolean() const { return jNodeVariant->isBoolean(); }
  [[nodiscard]] bool isNull() const { return jNodeVariant->isNull(); }
  [[nodiscard]] bool isHole() const { return jNodeVariant->isHole(); }
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get reference to JNode variant
  Variant &getVariant() { return *jNodeVariant; }
  const Variant &getVariant() const { return *jNodeVariant; }
  // Make JNode
  template<typename T, typename... Args> static auto make(Args &&...args)
  {
    return JNode{ std::make_unique<T>(std::forward<Args>(args)...) };
  }

private:
  JNode typeToJNode(const JSON::intializerListTypes &type);
  std::unique_ptr<Variant> jNodeVariant;
};
}// namespace JSON_Lib