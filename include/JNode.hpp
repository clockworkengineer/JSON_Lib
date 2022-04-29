#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    //
    // JNode structure.
    //
    enum class JNodeType
    {
        base = 0,
        object = 1,
        array = 2,
        number = 3,
        string = 4,
        boolean = 5,
        null = 6
    };
    //
    // Base JNode/
    //
    struct JNode
    {
        struct Error : public std::exception
        {
        public:
            explicit Error(std::string errorMessage) : errorMessage(std::move(errorMessage)) {}
            [[nodiscard]] const char *what() const noexcept override
            {
                return (errorMessage.c_str());
            }

        private:
            const std::string errorMessage;
        };
        explicit JNode(JNodeType nodeType = JNodeType::base) : nodeType(nodeType)
        {
        }
        virtual ~JNode() = default;
        JNode &operator[](const std::string &key);
        JNode &operator[](int index);
        const JNodeType nodeType;
    };
    //
    // Pointer to JNode
    //
    using JNodePtr = std::unique_ptr<JNode>;
    //
    // Dictionary JNode.
    //
    struct JNodeObject : JNode
    {
        using Entry = std::pair<std::string, JNodePtr>;
        explicit JNodeObject(std::vector<JNodeObject::Entry> &value) : JNode(JNodeType::object),
                                                                       m_value(std::move(value))
        {
        }
        [[nodiscard]] bool contains(const std::string &key) const
        {
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const Entry &entry) -> bool
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
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const Entry &entry) -> bool
                                       { return (entry.first == key); });
                it != m_value.end())
            {
                return (*it->second);
            }
            throw JNode::Error("JNode Error: Invalid key used to access object.");
        }
        const std::vector<Entry> &objects()
        {
            return (m_value);
        }

    private:
        const std::vector<JNodeObject::Entry> m_value;
    };
    //
    // Array JNode.
    //
    struct JNodeArray : JNode
    {
        explicit JNodeArray(std::vector<JNodePtr> &value) : JNode(JNodeType::array),
                                                            m_value(std::move(value))
        {
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        const std::vector<JNodePtr> &array() const
        {
            return (m_value);
        }
        JNode &operator[](int index) const
        {
            if ((index >= 0) && (index < (static_cast<int>(m_value.size()))))
            {
                return (*m_value[index]);
            }
            throw JNode::Error("JNode Error: Invalid index used to access array.");
        }

    private:
        const std::vector<JNodePtr> m_value;
    };
    //
    // Number JNode.
    //
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
        const std::string m_value;
    };
    //
    // String JNode.
    //
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
        const std::string m_value;
    };
    //
    // Boolean JNode.
    //
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
        const bool m_value;
    };
    //
    // Boolean JNode.
    //
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
    //
    //
    // Convert base JNode reference
    template <typename T>
    T &JNodeRef(JNode &jNode)
    {
        return (static_cast<T &>(jNode));
    }
    //
    // Index overloads
    //
    inline JNode &JNode::operator[](const std::string &key) // Object
    {
        if (nodeType == JNodeType::object)
        {
            return (JNodeRef<JNodeObject>(*this)[key]);
        }
        throw JNode::Error("JNode Error : Node not an object.");
    }
    inline JNode &JNode::operator[](int index) // Array
    {
        if (nodeType == JNodeType::array)
        {
            return (JNodeRef<JNodeArray>(*this)[index]);
        }
        throw JNode::Error("BNode Error: Node not a list.");
    }

} // namespace JSONLib