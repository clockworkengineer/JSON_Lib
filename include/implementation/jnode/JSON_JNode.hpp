#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// =====
// JNode
// =====
struct JNode
{
  // JNode Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  JNode() = default;
  template<typename T> explicit JNode(T value);
  JNode(const JSON::ArrayList &arrayList);
  JNode(const JSON::ObjectList &objectList);
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  template<typename T> JNode &operator=(T value) { return (*this = JNode(value)); }
  // Interrogate variant
  bool isEmpty() const { return (m_variant == nullptr); }
  bool isObject() const { return (m_variant->getType() == Variant::Type::object); }
  bool isArray() const { return (m_variant->getType() == Variant::Type::array); }
  bool isNumber() const { return (m_variant->getType() == Variant::Type::number); }
  bool isString() const { return (m_variant->getType() == Variant::Type::string); }
  bool isBoolean() const { return (m_variant->getType() == Variant::Type::boolean); }
  bool isNull() const { return (m_variant->getType() == Variant::Type::null); }
  bool isHole() const { return (m_variant->getType() == Variant::Type::hole); }
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get reference to JNode variant
  Variant &getVariant() { return (*m_variant); }
  const Variant &getVariant() const { return (*m_variant); }
  // Make JNode
  template<typename T, typename U> static JNode make(U &&value) { return (JNode{ std::make_unique<T>(T{ value }) }); }
  template<typename T> static JNode make() { return (JNode{ std::make_unique<T>() }); }

private:
  static JNode internalTypeToJNode(const JSON::InternalType &type);
  std::unique_ptr<Variant> m_variant;
};
}// namespace JSON_Lib