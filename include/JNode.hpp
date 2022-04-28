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
        JNodeObject() : JNode(JNodeType::object) {}
        [[nodiscard]] bool containsKey(const std::string &key) const
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
        void addEntry(const std::string &key, JNodePtr entry)
        {
            if (!containsKey(key))
            {
                m_value.emplace_back(key, std::move(entry));
            }
        }
        JNode *getEntry(const std::string &key)
        {
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const Entry &entry) -> bool
                                       { return (entry.first == key); });
                it != m_value.end())
            {
                return (it->second.get());
            }
            return (nullptr);
        }
        std::vector<Entry> &getObject()
        {
            return (m_value);
        }

    private:
        std::vector<JNodeObject::Entry> m_value;
    };
    //
    // List JNode.
    //
    struct JNodeArray : JNode
    {
        JNodeArray() : JNode(JNodeType::array) {}
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        void addEntry(JNodePtr jNode)
        {
            m_value.push_back(std::move(jNode));
        }
        std::vector<JNodePtr> &getArray()
        {
            return (m_value);
        }
        JNode *getEntry(int index)
        {
            return (m_value[index].get());
        }

    private:
        std::vector<JNodePtr> m_value;
    };
    //
    // Number JNode.
    //
    struct JNodeNumber : JNode
    {
        explicit JNodeNumber(const std::string &value) : JNode(JNodeType::number)
        {
            this->m_value = value;
        }
        // Convert to long returning true on success
        // Note: Can still return a long value for floating point
        // but false as the number is not in integer format
        bool getInteger(long long &longValue) const
        {
            char *end = nullptr;
            longValue = std::strtoll(m_value.c_str(), &end, 10);
            return (*end == '\0'); // If not all characters used then not success
        }
        // Convert to double returning true on success
        bool getFloatingPoint(double &doubleValue) const
        {
            char *end = nullptr;
            doubleValue = std::strtod(m_value.c_str(), &end);
            return (*end == '\0'); // If not all characters used then not success
        }
        std::string &getNumber()
        {
            return (m_value);
        }

    private:
        std::string m_value;
    };
    //
    // String JNode.
    //
    struct JNodeString : JNode
    {
        explicit JNodeString(const std::string &value) : JNode(JNodeType::string)
        {
            this->m_value = value;
        }
        std::string &getString()
        {
            return (m_value);
        }

    private:
        std::string m_value;
    };
    //
    // Boolean JNode.
    //
    struct JNodeBoolean : JNode
    {
        explicit JNodeBoolean(bool value) : JNode(JNodeType::boolean)
        {
            this->m_value = value;
        }
        [[nodiscard]] bool getBoolean() const
        {
            return (m_value);
        }

    private:
        bool m_value;
    };
    //
    // Boolean JNode.
    //
    struct JNodeNull : JNode
    {
        JNodeNull() : JNode(JNodeType::null)
        {
        }
        static void *getNull()
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
            if (JNodeRef<JNodeObject>(*this).containsKey(key))
            {
                return (*(JNodeRef<JNodeObject>(*this).getEntry(key)));
            }
        }
        throw Error("Invalid key used to access object.");
    }
    inline JNode &JNode::operator[](int index) // Array
    {
        if (nodeType == JNodeType::array)
        {
            if ((index >= 0) && (index < (JNodeRef<JNodeArray>(*this).size())))
            {
                return (*((JNodeRef<JNodeArray>(*this).getEntry(index))));
            }
        }
        throw Error("Invalid index used to access array.");
    }

} // namespace JSONLib