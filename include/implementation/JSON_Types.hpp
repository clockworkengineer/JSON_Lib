#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // =================
    // JSON Syntax Error
    // =================
    struct SyntaxError : public std::runtime_error
    {
        SyntaxError(std::string const &message) : std::runtime_error("JSON Error: " + message)
        {
        }
    };
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
            Error(std::string const &message) : std::runtime_error("JNode Error: " + message)
            {
            }
        };
        explicit JNode(JNodeType nodeType = JNodeType::base) : nodeType(nodeType)
        {
        }
        virtual ~JNode() = default;
        JNode &operator[](const std::string &key);
        JNode &operator[](int index);
        JNodeType nodeType;
    };
    // ==========
    // Dictionary
    // ==========
    struct JNodeObject : JNode
    {
        using KeyValuePair = std::pair<std::string, JNode::Ptr>;
        explicit JNodeObject(std::vector<JNodeObject::KeyValuePair> &value) : JNode(JNodeType::object),
                                                                              m_value(std::move(value))
        {
        }
        [[nodiscard]] bool contains(const std::string &key) const
        {
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const KeyValuePair &entry) -> bool
                                       { return (entry.first == key); });
                it != m_value.end())
            {
                return (true);
            }
            return (false);
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        JNode &operator[](const std::string &key) const
        {
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const KeyValuePair &entry) -> bool
                                       { return (entry.first == key); });
                it != m_value.end())
            {
                return (*it->second);
            }
            throw JNode::Error("Invalid key used to access object.");
        }
        const std::vector<KeyValuePair> &objects() const
        {
            return (m_value);
        }

    private:
        std::vector<JNodeObject::KeyValuePair> m_value;
    };
    // =====
    // Array
    // =====
    struct JNodeArray : JNode
    {
        explicit JNodeArray(std::vector<JNode::Ptr> &value) : JNode(JNodeType::array),
                                                              m_value(std::move(value))
        {
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        const std::vector<JNode::Ptr> &array() const
        {
            return (m_value);
        }
        JNode &operator[](int index) const
        {
            if ((index >= 0) && (index < (static_cast<int>(m_value.size()))))
            {
                return (*m_value[index]);
            }
            throw JNode::Error("Invalid index used to access array.");
        }

    private:
        std::vector<JNode::Ptr> m_value;
    };
    // ======
    // Number
    // ======
    struct JNodeNumber : JNode
    {
        explicit JNodeNumber(const std::string &value) : JNode(JNodeType::number), m_value(value)
        {
        }
        // Convert to long returning true on success
        // Note: Can still return a long value for floating point
        // but false as the number is not in integer format
        bool integer(long long &longValue) const
        {
            char *end = nullptr;
            longValue = std::strtoll(m_value.c_str(), &end, 10);
            return (*end == '\0'); // If not all characters used then not success
        }
        // Convert to double returning true on success
        bool floatingpoint(double &doubleValue) const
        {
            char *end = nullptr;
            doubleValue = std::strtod(m_value.c_str(), &end);
            return (*end == '\0'); // If not all characters used then not success
        }
        std::string number() const
        {
            return (m_value);
        }

    private:
        std::string m_value;
    };
    // ======
    // String
    // ======
    struct JNodeString : JNode
    {
        explicit JNodeString(const std::string &value) : JNode(JNodeType::string), m_value(value)
        {
        }
        std::string string() const
        {
            return (m_value);
        }

    private:
        std::string m_value;
    };
    // =======
    // Boolean
    // =======
    struct JNodeBoolean : JNode
    {
        explicit JNodeBoolean(bool value) : JNode(JNodeType::boolean), m_value(value)
        {
        }
        [[nodiscard]] bool boolean() const
        {
            return (m_value);
        }

    private:
        bool m_value;
    };
    // ====
    // Null
    // ====
    struct JNodeNull : JNode
    {
        JNodeNull() : JNode(JNodeType::null)
        {
        }
        void *null() const
        {
            return (nullptr);
        }
    };
    // ==============================
    // JNode base reference converter
    // ==============================
    template <typename T>
    void CheckJNodeType(const JNode &jNode)
    {
        if constexpr (std::is_same_v<T, JNodeString>)
        {
            if (jNode.nodeType != JNodeType::string)
            {
                throw JNode::Error("Node not a string.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNumber>)
        {
            if (jNode.nodeType != JNodeType::number)
            {
                throw JNode::Error("Node not a number.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeArray>)
        {
            if (jNode.nodeType != JNodeType::array)
            {
                throw JNode::Error("Node not an array.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeObject>)
        {
            if (jNode.nodeType != JNodeType::object)
            {
                throw JNode::Error("Node not an object.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeBoolean>)
        {
            if (jNode.nodeType != JNodeType::boolean)
            {
                throw JNode::Error("Node not an boolean.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNull>)
        {
            if (jNode.nodeType != JNodeType::null)
            {
                throw JNode::Error("Node not a null.");
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
    inline JNode &JNode::operator[](int index) // Array
    {
        return (JNodeRef<JNodeArray>(*this)[index]);
    }
} // namespace JSONLib