#pragma once
//
// C++ STL
//
#include <string>
#include <map>
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
    public:
        explicit JNode(JNodeType nodeType = JNodeType::base) : nodeType(nodeType)
        {
        }
        virtual ~JNode() = default;
        JNode &operator[](const std::string& key);
        JNode &operator[](int index);
        const JNodeType nodeType;
    };
    //
    // Dictionary JNode.
    //
    struct JNodeObject : JNode
    {
    public:
        JNodeObject() : JNode(JNodeType::object) {}
        [[nodiscard]] bool containsKey(const std::string &key) const
        {
            return (m_value.count(key) > 0);
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        void addEntry(const std::string& key, std::unique_ptr<JNode> entry)
        {
            m_value[key] = std::move(entry);
            m_keys.push_back(key);
        }
        JNode *getEntry(const std::string &key)
        {
            return (m_value[key].get());
        }
        std::vector<std::string> &getKeys()
        {
            return (m_keys);
        }

    protected:
        std::map<std::string, std::unique_ptr<JNode>> m_value;
        // Note: Store keys so when write away keep key order
        // that they have had in the source form be it file/network/buffer.
        std::vector<std::string> m_keys;
    };
    //
    // List JNode.
    //
    struct JNodeArray : JNode
    {
    public:
        JNodeArray() : JNode(JNodeType::array) {}
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        void addEntry(std::unique_ptr<JNode> jNode)
        {
            m_value.push_back(std::move(jNode));
        }
        std::vector<std::unique_ptr<JNode>> &getArray()
        {
            return (m_value);
        }
        JNode *getEntry(int index)
        {
            return (m_value[index].get());
        }

    protected:
        std::vector<std::unique_ptr<JNode>> m_value;
    };
    //
    // Number JNode.
    //
    struct JNodeNumber : JNode
    {
    public:
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

    protected:
        std::string m_value;
    };
    //
    // String JNode.
    //
    struct JNodeString : JNode
    {
    public:
        explicit JNodeString(const std::string &value) : JNode(JNodeType::string)
        {
            this->m_value = value;
        }
        std::string &getString()
        {
            return (m_value);
        }

    protected:
        std::string m_value;
    };
    //
    // Boolean JNode.
    //
    struct JNodeBoolean : JNode
    {
    public:
        explicit JNodeBoolean(bool value) : JNode(JNodeType::boolean)
        {
            this->m_value = value;
        }
        [[nodiscard]] bool getBoolean() const
        {
            return (m_value);
        }

    protected:
        bool m_value;
    };
    //
    // Boolean JNode.
    //
    struct JNodeNull : JNode
    {
    public:
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
    inline JNode &JNode::operator[](const std::string& key) // Object
    {
        if (nodeType == JNodeType::object)
        {
            if (JNodeRef<JNodeObject>(*this).containsKey(key))
            {
                return (*(JNodeRef<JNodeObject>(*this).getEntry(key)));
            }
        }
        throw std::runtime_error("Invalid key used to access object.");
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
        throw std::runtime_error("Invalid index used to access array.");
    }

} // namespace JSONLib