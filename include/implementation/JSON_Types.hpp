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
    struct JNodeData
    {
        explicit JNodeData(JNodeType nodeType = JNodeType::base) : m_nodeType(nodeType)
        {
        }
        [[nodiscard]] JNodeType getNodeType() const
        {
            return (m_nodeType);
        }

    private:
        JNodeType m_nodeType;
    };
    struct JNode
    {
        using Ptr = std::unique_ptr<JNode>;
        struct Error : public std::runtime_error
        {
            explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message)
            {
            }
        };

        explicit JNode() {}
        JNode &operator[](const std::string &key);
        const JNode &operator[](const std::string &key) const;
        JNode &operator[](int index);
        const JNode &operator[](int index) const;
        [[nodiscard]] JNodeType getNodeType() const
        {
            return (m_jNodeData->getNodeType());
        }

        std::unique_ptr<JNodeData> &getJNodeData()
        {
            return (m_jNodeData);
        }
        const std::unique_ptr<JNodeData> &getJNodeData() const
        {
            return (m_jNodeData);
        }

    private:
        std::unique_ptr<JNodeData> m_jNodeData;
    };
    struct JNodeObjectEntry
    {
        std::string key;
        JNode::Ptr value;
    };
    // ==========
    // JNode Data
    // ==========
    struct JNodeObjectData : JNodeData
    {
        JNodeObjectData(std::vector<JNodeObjectEntry> &objects) : JNodeData(JNodeType::object), m_jsonObjects(std::move(objects))
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
            catch ([[maybe_unused]] const JNode::Error &e)
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
                throw JNode::Error("Invalid key used to access object.");
            }
            return (entry);
        }
        std::vector<JNodeObjectEntry> m_jsonObjects;
    };
    struct JNodeArrayData : JNodeData
    {
        JNodeArrayData(std::vector<JNode::Ptr> &array) : JNodeData(JNodeType::array), m_jsonArray(std::move(array))
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
            throw JNode::Error("Invalid index used to access array.");
        }
        const JNode &operator[](int index) const
        {
            if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size()))))
            {
                return (*m_jsonArray[index]);
            }
            throw JNode::Error("Invalid index used to access array.");
        }

    private:
        std::vector<JNode::Ptr> m_jsonArray;
    };
    struct JNodeNumberData : JNodeData
    {
        JNodeNumberData()
        {
        }
        JNodeNumberData(std::array<char, kLongLongWidth> &number) : JNodeData(JNodeType::number), m_jsonNumber(std::move(number))
        {
        }
        bool isValidNumeric(char ch)
        {
            // Includes possible sign, decimal point or exponent
            return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' || ch == 'E' || ch == 'e');
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

    private:
        std::array<char, kLongLongWidth> m_jsonNumber{};
    };
    struct JNodeStringData : JNodeData
    {
        JNodeStringData(const std::string &string) : JNodeData(JNodeType::string), m_jsonString(std::move(string))
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
    struct JNodeBooleanData : JNodeData
    {
        JNodeBooleanData(bool boolean) : JNodeData(JNodeType::boolean), m_jsonBoolean(boolean)
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
    struct JNodeNullData : JNodeData
    {
        JNodeNullData() : JNodeData(JNodeType::null)
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
    // ==============
    // JNode Creation
    // ==============
    inline std::unique_ptr<JNode> makeJNodeObject(std::vector<JNodeObjectEntry> &objects)
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeObjectData>(std::move(JNodeObjectData{objects}));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeArray(std::vector<JNode::Ptr> &array)
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeArrayData>(std::move(JNodeArrayData{array}));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeNumber(std::array<char, kLongLongWidth> &number)
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{number}));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeString(const std::string &string)
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeStringData>(std::move(JNodeStringData{string}));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeBoolean(bool boolean)
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeBooleanData>(std::move(JNodeBooleanData{boolean}));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeNull()
    {
        JNode jNode;
        jNode.getJNodeData() = std::make_unique<JNodeNullData>(std::move(JNodeNullData()));
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    // ==============================
    // JNode base reference converter
    // ==============================
    template <typename T>
    void CheckJNodeDataType(const JNodeData &jNodeData)
    {
        if constexpr (std::is_same_v<T, JNodeStringData>)
        {
            if (jNodeData.getNodeType() != JNodeType::string)
            {
                throw JNode::Error("Node not a string.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNumberData>)
        {
            if (jNodeData.getNodeType() != JNodeType::number)
            {
                throw JNode::Error("Node not a number.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeArrayData>)
        {
            if (jNodeData.getNodeType() != JNodeType::array)
            {
                throw JNode::Error("Node not an array.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeObjectData>)
        {
            if (jNodeData.getNodeType() != JNodeType::object)
            {
                throw JNode::Error("Node not an object.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeBooleanData>)
        {
            if (jNodeData.getNodeType() != JNodeType::boolean)
            {
                throw JNode::Error("Node not an boolean.");
            }
        }
        else if constexpr (std::is_same_v<T, JNodeNullData>)
        {
            if (jNodeData.getNodeType() != JNodeType::null)
            {
                throw JNode::Error("Node not a null.");
            }
        }
    }
    template <typename T>
    T &JNodeDataRef(JNode &jNode)
    {
        CheckJNodeDataType<T>(*jNode.getJNodeData());
        return (static_cast<T &>(*jNode.getJNodeData()));
    }
    template <typename T>
    const T &JNodeDataRef(const JNode &jNode)
    {
        CheckJNodeDataType<T>(*jNode.getJNodeData());
        return (static_cast<const T &>(*jNode.getJNodeData()));
    }
    // ===============
    // Index overloads
    // ===============
    inline JNode &JNode::operator[](const std::string &key) // Object
    {
        return (JNodeDataRef<JNodeObjectData>(*this)[key]);
    }
    inline const JNode &JNode::operator[](const std::string &key) const // Object
    {
        return (JNodeDataRef<const JNodeObjectData>(*this)[key]);
    }
    inline JNode &JNode::operator[](int index) // Array
    {
        return (JNodeDataRef<JNodeArrayData>(*this)[index]);
    }
    inline const JNode &JNode::operator[](int index) const // Array
    {
        return (JNodeDataRef<JNodeArrayData>(*this)[index]);
    }
} // namespace JSONLib