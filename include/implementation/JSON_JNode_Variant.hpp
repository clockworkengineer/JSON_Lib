#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==============
// JNode variants
// ==============
// ====
// Base
// ====
struct Variant {
  explicit Variant(JNodeType nodeType = JNodeType::base)
      : m_nodeType(nodeType) {}
  [[nodiscard]] JNodeType getNodeType() const { return (m_nodeType); }
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;

private:
  JNodeType m_nodeType;
};
// ======
// Object
// ======
struct Object : Variant {
  // Object entry
  struct Entry {
    std::string key;
    JNode::Ptr value;
  };
  // Object entry list
  using EntryList = std::vector<Object::Entry>;
  // Constructors/Destructors
  Object() : Variant(JNodeType::object) {}
  explicit Object(EntryList &objects)
      : Variant(JNodeType::object), m_jsonObjects(std::move(objects)) {}
  Object(const Object &other) = delete;
  Object &operator=(const Object &other) = delete;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;
  // Search for a given entry given a key and object list
  static auto findKey(const std::string &key, const EntryList &objects) {
    auto entry = std::find_if(objects.begin(), objects.end(),
                              [&key](const Entry &entry) -> bool {
                                return (entry.key == key);
                              });
    if (entry == objects.end()) {
      throw JNode::Error("Invalid key used to access object.");
    }
    return (entry);
  }
  // Find a given object entry given its key
  [[nodiscard]] auto find(const std::string &key) const {
    return (findKey(key, m_jsonObjects));
  }
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string &key) const {
    try {
      [[maybe_unused]] auto entry = findKey(key, m_jsonObjects);
    } catch ([[maybe_unused]] const JNode::Error &e) {
      return (false);
    }
    return (true);
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const {
    return (static_cast<int>(m_jsonObjects.size()));
  }
  // Return object entry for a given key
  JNode &operator[](const std::string &key) {
    return (*(findKey(key, m_jsonObjects)->value));
  }
  const JNode &operator[](const std::string &key) const {
    return (*(findKey(key, m_jsonObjects)->value));
  }
  // Return reference to base of object entries
  EntryList &objects() { return (m_jsonObjects); }
  [[nodiscard]] const EntryList &objects() const { return (m_jsonObjects); }

private:
  EntryList m_jsonObjects;
};
// =====
// Array
// =====
struct Array : Variant {
  // Array entry list
  using ArrayList = std::vector<JNode::Ptr>;
  // Constructors/Destructors
  Array() : Variant(JNodeType::array) {}
  explicit Array(ArrayList &array)
      : Variant(JNodeType::array), m_jsonArray(std::move(array)) {}
  Array(const Array &other) = delete;
  Array &operator=(const Array &other) = delete;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return (m_jsonArray.size()); }
  // Return reference to array base
  ArrayList &array() { return (m_jsonArray); }
  [[nodiscard]] const ArrayList &array() const { return (m_jsonArray); }
  // Array indexing operators
  JNode &operator[](std::size_t index) {
    if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size())))) {
      return (*m_jsonArray[index]);
    }
    throw JNode::Error("Invalid index used to access array.");
  }
  const JNode &operator[](std::size_t index) const {
    if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size())))) {
      return (*m_jsonArray[index]);
    }
    throw JNode::Error("Invalid index used to access array.");
  }

private:
  ArrayList m_jsonArray;
};
// ======
// Number
// ======
struct Number : Variant {
  // Constructors/Destructors
  Number() : Variant(JNodeType::number) {}
  explicit Number(const Numeric &number)
      : Variant(JNodeType::number), m_jsonNumber(number) {}
  Number(const Number &other) = delete;
  Number &operator=(const Number &other) = delete;
  Number(Number &&other) = default;
  Number &operator=(Number &&other) = default;
  ~Number() = default;
  // Return reference to number string
  [[nodiscard]] Numeric &number() { return (m_jsonNumber); }
  [[nodiscard]] const Numeric &number() const { return (m_jsonNumber); }
  // Convert number to string
  [[nodiscard]] std::string toString() const {
    return (m_jsonNumber.getString());
  }

private:
  Numeric m_jsonNumber{};
};
// ======
// String
// ======
struct String : Variant {
  // Constructors/Destructors
  String() : Variant(JNodeType::string) {}
  explicit String(std::string string)
      : Variant(JNodeType::string), m_jsonString(std::move(string)) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Return reference to string
  std::string &string() { return (m_jsonString); }
  [[nodiscard]] const std::string &string() const { return (m_jsonString); }
  // Convert string representation to a string
  [[nodiscard]] std::string toString() const { return (m_jsonString); }

private:
  std::string m_jsonString;
};
// =======
// Boolean
// =======
struct Boolean : Variant {
  // Constructors/Destructors
  Boolean() : Variant(JNodeType::boolean) {}
  explicit Boolean(bool boolean)
      : Variant(JNodeType::boolean), m_jsonBoolean(boolean) {}
  Boolean(const Boolean &other) = delete;
  Boolean &operator=(const Boolean &other) = delete;
  Boolean(Boolean &&other) = default;
  Boolean &operator=(Boolean &&other) = default;
  ~Boolean() = default;
  // Return boolean value
  [[nodiscard]] bool boolean() const { return (m_jsonBoolean); }
  // Return string representation of boolean value
  [[nodiscard]] std::string toString() const {
    return (m_jsonBoolean ? "true" : "false");
  }

private:
  bool m_jsonBoolean{};
};
// ====
// Null
// ====
struct Null : Variant {
  // Constructors/Destructors
  Null() : Variant(JNodeType::null) {}
  Null(const Null &other) = delete;
  Null &operator=(const Null &other) = delete;
  Null(Null &&other) = default;
  Null &operator=(Null &&other) = default;
  ~Null() = default;
  // Return null value
  [[nodiscard]] void *null() const { return (nullptr); }
  // Return string representation of null value
  [[nodiscard]] std::string toString() const { return ("null"); }
};
// ====
// Hole
// ====
struct Hole : Variant {
  // Constructors/Destructors
  Hole() : Variant(JNodeType::hole) {}
  Hole(const Hole &other) = delete;
  Hole &operator=(const Hole &other) = delete;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
  [[nodiscard]] std::string toString() const { return ("null"); }
};
// =========================
// JNode reference converter
// =========================
template <typename T> void CheckJNodeType(const Variant &jNodeVariant) {
  if constexpr (std::is_same_v<T, String>) {
    if (jNodeVariant.getNodeType() != JNodeType::string) {
      throw JNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, Number>) {
    if (jNodeVariant.getNodeType() != JNodeType::number) {
      throw JNode::Error("Node not a number.");
    }
  } else if constexpr (std::is_same_v<T, Array>) {
    if (jNodeVariant.getNodeType() != JNodeType::array) {
      throw JNode::Error("Node not an array.");
    }
  } else if constexpr (std::is_same_v<T, Object>) {
    if (jNodeVariant.getNodeType() != JNodeType::object) {
      throw JNode::Error("Node not an object.");
    }
  } else if constexpr (std::is_same_v<T, Boolean>) {
    if (jNodeVariant.getNodeType() != JNodeType::boolean) {
      throw JNode::Error("Node not an boolean.");
    }
  } else if constexpr (std::is_same_v<T, Null>) {
    if (jNodeVariant.getNodeType() != JNodeType::null) {
      throw JNode::Error("Node not a null.");
    }
  }
}
template <typename T> T &JNodeRef(JNode &jNode) {
  CheckJNodeType<T>(*jNode.getJNodeVariant());
  return (static_cast<T &>(*jNode.getJNodeVariant()));
}
template <typename T> const T &JNodeRef(const JNode &jNode) {
  CheckJNodeType<T>(*jNode.getJNodeVariant());
  return (static_cast<const T &>(*jNode.getJNodeVariant()));
}
} // namespace JSONLib