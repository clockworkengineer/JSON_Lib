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
// ===============
// JNode  Variants
// ===============
// ====
// Base
// ====
struct JNodeVariant {
  explicit JNodeVariant(JNodeType nodeType = JNodeType::base)
      : m_nodeType(nodeType) {}
  [[nodiscard]] JNodeType getNodeType() const { return (m_nodeType); }
  JNodeVariant(const JNodeVariant &other) = delete;
  JNodeVariant &operator=(const JNodeVariant &other) = delete;
  JNodeVariant(JNodeVariant &&other) = default;
  JNodeVariant &operator=(JNodeVariant &&other) = default;
  ~JNodeVariant() = default;

private:
  JNodeType m_nodeType;
};

// ======
// Object
// ======
struct JNodeObject : JNodeVariant {
  // Object entry
  struct ObjectEntry {
    std::string key;
    JNode::Ptr value;
  };
  // Object entry list
  using ObjectList = std::vector<JNodeObject::ObjectEntry>;
  // Constructors/Destructors
  JNodeObject() : JNodeVariant(JNodeType::object) {}
  explicit JNodeObject(ObjectList &objects)
      : JNodeVariant(JNodeType::object), m_jsonObjects(std::move(objects)) {}
  JNodeObject(const JNodeObject &other) = delete;
  JNodeObject &operator=(const JNodeObject &other) = delete;
  JNodeObject(JNodeObject &&other) = default;
  JNodeObject &operator=(JNodeObject &&other) = default;
  ~JNodeObject() = default;
  // Search for a given entry given a key and object list
  static auto findKey(const std::string &key, const ObjectList &objects) {
    auto entry =
        std::find_if(objects.begin(), objects.end(),
                     [&key](const JNodeObject::ObjectEntry &entry) -> bool {
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
  ObjectList &objects() { return (m_jsonObjects); }
  [[nodiscard]] const ObjectList &objects() const { return (m_jsonObjects); }

private:
  ObjectList m_jsonObjects;
};
// =====
// Array
// =====
struct JNodeArray : JNodeVariant {
  // Array entry list
  using ArrayList = std::vector<JNode::Ptr>;
  // Constructors/Destructors
  JNodeArray() : JNodeVariant(JNodeType::array) {}
  explicit JNodeArray(ArrayList &array)
      : JNodeVariant(JNodeType::array), m_jsonArray(std::move(array)) {}
  JNodeArray(const JNodeArray &other) = delete;
  JNodeArray &operator=(const JNodeArray &other) = delete;
  JNodeArray(JNodeArray &&other) = default;
  JNodeArray &operator=(JNodeArray &&other) = default;
  ~JNodeArray() = default;
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
struct JNodeNumber : JNodeVariant {
  // Constructors/Destructors
  JNodeNumber() : JNodeVariant(JNodeType::number) {}
  explicit JNodeNumber(const JNodeNumeric &number)
      : JNodeVariant(JNodeType::number), m_jsonNumber(number) {}
  JNodeNumber(const JNodeNumber &other) = delete;
  JNodeNumber &operator=(const JNodeNumber &other) = delete;
  JNodeNumber(JNodeNumber &&other) = default;
  JNodeNumber &operator=(JNodeNumber &&other) = default;
  ~JNodeNumber() = default;
  // Is character a valid numeric character ?
  [[nodiscard]] bool isValidNumeric(char ch) const {
    // Includes possible sign, decimal point or exponent
    return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' ||
            ch == 'E' || ch == 'e');
  }
  // Convert to long long returning true on success
  // Note: Can still return a long value for floating point
  // but false as the number is not in integer format
  [[nodiscard]] bool integer(long long &longValue) const {
    char *end = nullptr;
    longValue = std::strtoll(&m_jsonNumber.value[0], &end, 10);
    return (*end == '\0'); // If not all characters used then not success
  }
  // Convert to long double returning true on success
  [[nodiscard]] bool floatingpoint(long double &doubleValue) const {
    char *end = nullptr;
    doubleValue = std::strtod(&m_jsonNumber.value[0], &end);
    return (*end == '\0'); // If not all characters used then not success
  }
  // Check whether we nave a numeric value
  [[nodiscard]] bool isValidNumber() const {
    if ([[maybe_unused]] long long longValue{}; integer(longValue)) {
      return (true);
    }
    if ([[maybe_unused]] long double doubleValue{};
        floatingpoint(doubleValue)) {
      return (true);
    }
    return (false);
  }
  // Return reference to number character array
  [[nodiscard]] JNodeNumeric &number() { return (m_jsonNumber); }
  [[nodiscard]] const JNodeNumeric &number() const { return (m_jsonNumber); }
  // Convert number to string
  [[nodiscard]] std::string toString() const {
    return (std::string{
        m_jsonNumber.value.begin(),
        m_jsonNumber.value.begin() +
            static_cast<std::ptrdiff_t>(std::strlen(&m_jsonNumber.value[0]))});
  }

private:
  JNodeNumeric m_jsonNumber{};
};
// ======
// String
// ======
struct JNodeString : JNodeVariant {
  // Constructors/Destructors
  JNodeString() : JNodeVariant(JNodeType::string) {}
  explicit JNodeString(std::string string)
      : JNodeVariant(JNodeType::string), m_jsonString(std::move(string)) {}
  JNodeString(const JNodeString &other) = delete;
  JNodeString &operator=(const JNodeString &other) = delete;
  JNodeString(JNodeString &&other) = default;
  JNodeString &operator=(JNodeString &&other) = default;
  ~JNodeString() = default;
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
struct JNodeBoolean : JNodeVariant {
  // Constructors/Destructors
  JNodeBoolean() : JNodeVariant(JNodeType::boolean) {}
  explicit JNodeBoolean(bool boolean)
      : JNodeVariant(JNodeType::boolean), m_jsonBoolean(boolean) {}
  JNodeBoolean(const JNodeBoolean &other) = delete;
  JNodeBoolean &operator=(const JNodeBoolean &other) = delete;
  JNodeBoolean(JNodeBoolean &&other) = default;
  JNodeBoolean &operator=(JNodeBoolean &&other) = default;
  ~JNodeBoolean() = default;
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
struct JNodeNull : JNodeVariant {
  // Constructors/Destructors
  JNodeNull() : JNodeVariant(JNodeType::null) {}
  JNodeNull(const JNodeNull &other) = delete;
  JNodeNull &operator=(const JNodeNull &other) = delete;
  JNodeNull(JNodeNull &&other) = default;
  JNodeNull &operator=(JNodeNull &&other) = default;
  ~JNodeNull() = default;
  // Return null value
  [[nodiscard]] void *null() const { return (nullptr); }
  // Return string representation of null value
  [[nodiscard]] std::string toString() const { return ("null"); }
};
// ====
// Hole
// ====
struct JNodeHole : JNodeVariant {
  // Constructors/Destructors
  JNodeHole() : JNodeVariant(JNodeType::hole) {}
  JNodeHole(const JNodeHole &other) = delete;
  JNodeHole &operator=(const JNodeHole &other) = delete;
  JNodeHole(JNodeHole &&other) = default;
  JNodeHole &operator=(JNodeHole &&other) = default;
  ~JNodeHole() = default;
  [[nodiscard]] std::string toString() const { return ("null"); }
};
// =========================
// JNode reference converter
// =========================
template <typename T> void CheckJNodeType(const JNodeVariant &jNodeVariant) {
  if constexpr (std::is_same_v<T, JNodeString>) {
    if (jNodeVariant.getNodeType() != JNodeType::string) {
      throw JNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, JNodeNumber>) {
    if (jNodeVariant.getNodeType() != JNodeType::number) {
      throw JNode::Error("Node not a number.");
    }
  } else if constexpr (std::is_same_v<T, JNodeArray>) {
    if (jNodeVariant.getNodeType() != JNodeType::array) {
      throw JNode::Error("Node not an array.");
    }
  } else if constexpr (std::is_same_v<T, JNodeObject>) {
    if (jNodeVariant.getNodeType() != JNodeType::object) {
      throw JNode::Error("Node not an object.");
    }
  } else if constexpr (std::is_same_v<T, JNodeBoolean>) {
    if (jNodeVariant.getNodeType() != JNodeType::boolean) {
      throw JNode::Error("Node not an boolean.");
    }
  } else if constexpr (std::is_same_v<T, JNodeNull>) {
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