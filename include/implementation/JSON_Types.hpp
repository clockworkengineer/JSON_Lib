#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <stdexcept>
#include <limits>
#include <array>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ==========
    // JSON Error
    // ==========
    struct Error : public std::runtime_error
    {
        explicit Error(const std::string &message) : std::runtime_error("JSON Error: " + message)
        {
        }
    };
    // ======================================
    // Numeric values max width in characters
    // ======================================
    static constexpr int kLongLongWidth = std::numeric_limits<long long>::digits10 + 2;
    static constexpr int kLongDoubleWidth = std::numeric_limits<long double>::digits10 + 2;
    // ===========
    // JNode types
    // ===========
    enum class JNodeType
    {
        base = 0,
        object,
        array,
        number,
        string,
        boolean,
        null
    };
    // ====
    // Base
    // ====
    struct JNode
    {
        using Ptr = std::unique_ptr<JNode>;
        struct Error : public std::runtime_error
        {
            explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message)
            {
            }
        };
        explicit JNode(JNodeType nodeType = JNodeType::base) : m_nodeType(nodeType)
        {
        }
        [[nodiscard]] JNodeType getNodeType() const
        {
            return (m_nodeType);
        }
        // No JNode is deleted through its base class so omit and save space
        // from virtual function table.
        // virtual ~JNode() = default;
        JNode &operator[](const std::string &key);
        const JNode &operator[](const std::string &key) const;
        JNode &operator[](int index);
        const JNode &operator[](int index) const;

    private:
        JNodeType m_nodeType;
    };
    // ==========
    // Dictionary
    // ==========
    struct JNodeObjectEntry
    {
        std::string key;
        JNode::Ptr value;
    };
    struct JNodeObject : JNode
    {
    public:
        JNodeObject() : JNode(JNodeType::object)
        {
        }
        explicit JNodeObject(std::vector<JNodeObjectEntry> &objects) : JNode(JNodeType::object),
                                                                       m_jsonObjects(std::move(objects))
        {
        }
        [[nodiscard]] auto find(const std::string &key) const
        {
            return (findKey(key, m_jsonObjects));
        }
        [[nodiscard]] bool contains(const std::string &key) const
        {
            try
            {
                [[maybe_unused]] auto entry = findKey(key, m_jsonObjects);
            }
            catch ([[maybe_unused]] const Error &e)
            {
                return (false);
            }
            return (true);
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_jsonObjects.size()));
        }
        JNode &operator[](const std::string &key)
        {
            return (*(findKey(key, m_jsonObjects)->value));
        }
        const JNode &operator[](const std::string &key) const
        {
            return (*(findKey(key, m_jsonObjects)->value));
        }
        std::vector<JNodeObjectEntry> &objects()
        {
            return (m_jsonObjects);
        }
        [[nodiscard]] const std::vector<JNodeObjectEntry> &objects() const
        {
            return (m_jsonObjects);
        }

    private:
        static std::vector<JNodeObjectEntry>::const_iterator findKey(const std::string &key, const std::vector<JNodeObjectEntry> &objects)
        {
            auto entry = std::find_if(objects.begin(), objects.end(), [&key](const JNodeObjectEntry &entry) -> bool
                                      { return (entry.key == key); });
            if (entry == objects.end())
            {
                throw Error("Invalid key used to access object.");
            }
            return (entry);
        }
        std::vector<JNodeObjectEntry> m_jsonObjects;
    };
    using JNodeObjectEntry = JNodeObjectEntry;
    // =====
    // Array
    // =====
    struct JNodeArray : JNode
    {
        JNodeArray() : JNode(JNodeType::array)
        {
        }
        explicit JNodeArray(std::vector<JNode::Ptr> &array) : JNode(JNodeType::array),
                                                              m_jsonArray(std::move(array))
        {
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_jsonArray.size()));
        }
        std::vector<JNode::Ptr> &array()
        {
            return (m_jsonArray);
        }
        [[nodiscard]] const std::vector<JNode::Ptr> &array() const
        {
            return (m_jsonArray);
        }
        JNode &operator[](int index)
        {
            if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size()))))
            {
                return (*m_jsonArray[index]);
            }
            throw Error("Invalid index used to access array.");
        }
        const JNode &operator[](int index) const
        {
            if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size()))))
            {
                return (*m_jsonArray[index]);
            }
            throw Error("Invalid index used to access array.");
        }

    private:
        std::vector<JNode::Ptr> m_jsonArray;
    };
    // ======
    // Number
    // ======
    struct JNodeNumber : JNode
    {
        JNodeNumber() : JNode(JNodeType::number)
        {
        }
        // Convert to long long returning true on success
        // Note: Can still return a long value for floating point
        // but false as the number is not in integer format
        [[nodiscard]] bool integer(long long &longValue) const
        {
            char *end = nullptr;
            longValue = std::strtoll(&m_jsonNumber[0], &end, 10);
            return (*end == '\0'); // If not all characters used then not success
        }
        // Convert to long double returning true on success
        [[nodiscard]] bool floatingpoint(long double &doubleValue) const
        {
            char *end = nullptr;
            doubleValue = std::strtod(&m_jsonNumber[0], &end);
            return (*end == '\0'); // If not all characters used then not success
        }
        // Check whether we nave a numeric value
        [[nodiscard]] bool isValidNumber() const
        {
            if ([[maybe_unused]] long long longValue{}; integer(longValue))
            {
                return (true);
            }
            if ([[maybe_unused]] long double doubleValue{}; floatingpoint(doubleValue))
            {
                return (true);
            }
            return (false);
        }
        [[nodiscard]] std::array<char, kLongLongWidth> &number()
        {
            return (m_jsonNumber);
        }
        [[nodiscard]] const std::array<char, kLongLongWidth> &number() const
        {
            return (m_jsonNumber);
        }
        [[nodiscard]] std::string toString() const
        {
            return (std::string{m_jsonNumber.begin(), m_jsonNumber.begin() + std::strlen(&m_jsonNumber[0])});
        }
        [[nodiscard]] bool isValidNumeric(char ch)
        {
            // Includes possible sign, decimal point or exponent
            return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' || ch == 'E' || ch == 'e');
        }

    private:
        std::array<char, kLongLongWidth> m_jsonNumber{};
    };
    // ======
    // String
    // ======
    struct JNodeString : JNode
    {
        JNodeString() : JNode(JNodeType::string)
        {
        }
        explicit JNodeString(std::string str) : JNode(JNodeType::string), m_jsonString(std::move(str))
        {
        }
        std::string &string()
        {
            return (m_jsonString);
        }
        [[nodiscard]] const std::string &string() const
        {
            return (m_jsonString);
        }
        [[nodiscard]] std::string toString() const
        {
            return (m_jsonString);
        }

    private:
        std::string m_jsonString;
    };
    // =======
    // Boolean
    // =======
    struct JNodeBoolean : JNode
    {
        explicit JNodeBoolean(bool boolean) : JNode(JNodeType::boolean), m_jsonBoolean(boolean)
        {
        }
        [[nodiscard]] bool boolean() const
        {
            return (m_jsonBoolean);
        }
        [[nodiscard]] std::string toString() const
        {
            return (m_jsonBoolean ? "true" : "false");
        }

    private:
        bool m_jsonBoolean;
    };
    // ====
    // Null
    // ====
    struct JNodeNull : JNode
    {
        JNodeNull() : JNode(JNodeType::null)
        {
        }
        [[nodiscard]] void *null() const
        {
            return (nullptr);
        }
        [[nodiscard]] std::string toString() const
        {
            return ("null");
        }
    };
    // ==============================
    // JNode base reference converter
    // ==============================
    template <typename T>
    void CheckJNodeType(auto &jNode)
    {
        if constexpr (std::is_same_v<T, JNodeString>)
        {
            if (jNode.getNodeType() != JNodeType::string)
            {
                throw Error("Node not a string.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNumber>)
        {
            if (jNode.getNodeType() != JNodeType::number)
            {
                throw Error("Node not a number.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeArray>)
        {
            if (jNode.getNodeType() != JNodeType::array)
            {
                throw Error("Node not an array.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeObject>)
        {
            if (jNode.getNodeType() != JNodeType::object)
            {
                throw Error("Node not an object.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeBoolean>)
        {
            if (jNode.getNodeType() != JNodeType::boolean)
            {
                throw Error("Node not an boolean.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNull>)
        {
            if (jNode.getNodeType() != JNodeType::null)
            {
                throw Error("Node not a null.");
            }
        }
    }
    template <typename T>
    T &JNodeRef(JNode &jNode)
    {
        CheckJNodeType<T>(jNode);
        return (static_cast<T &>(jNode));
    }
    template <typename T>
    const T &JNodeRef(const JNode &jNode)
    {
        CheckJNodeType<T>(jNode);
        return (static_cast<const T &>(jNode));
    }
    // ===============
    // Index overloads
    // ===============
    inline JNode &JNode::operator[](const std::string &key) // Object
    {
        return (JNodeRef<JNodeObject>(*this)[key]);
    }
    inline const JNode &JNode::operator[](const std::string &key) const // Object
    {
        return (JNodeRef<const JNodeObject>(*this)[key]);
    }
    inline JNode &JNode::operator[](int index) // Array
    {
        return (JNodeRef<JNodeArray>(*this)[index]);
    }
    inline const JNode &JNode::operator[](int index) const // Array
    {
        return (JNodeRef<JNodeArray>(*this)[index]);
    }
} // namespace JSONLib