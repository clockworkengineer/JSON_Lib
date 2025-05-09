#pragma once

namespace JSON_Lib {

struct JNode
{
  // JNode Error
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("JNode Error: ").append(message)) {}
  };
  // Constructors/Destructors
  JNode() = default;
  template<typename T> explicit JNode(T value);
  JNode(const JSON::ArrayInitializer &array);
  JNode(const JSON::ObjectInitializer &object);
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  template<typename T> JNode &operator=(T value) { return *this = JNode(value); }
  // Has the variant been created
  [[nodiscard]] bool isEmpty() const { return jNodeVariant == nullptr; }
  // Indexing operators
  JNode &operator[](const std::string_view &key);
  const JNode &operator[](const std::string_view &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get reference to JNode variant
  Variant &getVariant() { return *jNodeVariant; }
  [[nodiscard]] const Variant &getVariant() const { return *jNodeVariant; }
  // Make JNode
  template<typename T, typename... Args> static auto make(Args &&...args)
  {
    return JNode{ std::make_unique<T>(std::forward<Args>(args)...) };
  }

private:
  std::unique_ptr<Variant> jNodeVariant;
};
}// namespace JSON_Lib