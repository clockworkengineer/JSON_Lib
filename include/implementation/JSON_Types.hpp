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
#include <cstring>
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
    // ===========
    // JNode Types
    // ===========
    enum class JNodeType
    {
        base = 0,
        object,
        array,
        number,
        string,
        boolean,
        null,
        hole
    };
    // ============
    // JNode Number
    // ============
    // Numeric values max width in characters
    static constexpr int kLongLongWidth = std::numeric_limits<long long>::digits10 + 2;
    static constexpr int kLongDoubleWidth = std::numeric_limits<long double>::digits10 + 2;
    // Numbers stored in fix length array
    using JNodeNumber = std::array<char, kLongLongWidth>;
    // ==============
    // JNodeData Base
    // ==============
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
    // =====
    // JNode
    // =====
    struct JNode
    {
        // Pointer to JNode
        using Ptr = std::unique_ptr<JNode>;
        // JNode Error
        struct Error : public std::runtime_error
        {
            explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message)
            {
            }
        };
        // Constructors/Destructors
        JNode(std::unique_ptr<JNodeData> jNodeData) : m_jNodeData(std::move(jNodeData)) {}
        JNode(const JNode &other) = default;
        JNode &operator=(const JNode &other) = default;
        JNode(JNode &&other) = default;
        JNode &operator=(JNode &&other) = default;
        ~JNode() = default;
        // Assignment operators
        JNode &operator=(float floatingPoint);
        JNode &operator=(double floatingPoint);
        JNode &operator=(long double floatingPoint);
        JNode &operator=(int integer);
        JNode &operator=(long integer);
        JNode &operator=(long long integer);
        JNode &operator=(const char *cString);
        JNode &operator=(const std::string &string);
        JNode &operator=(bool boolean);
        JNode &operator=(std::nullptr_t null);
        // Indexing operators
        JNode &operator[](const std::string &key);
        const JNode &operator[](const std::string &key) const;
        JNode &operator[](std::size_t index);
        const JNode &operator[](std::size_t index) const;
        // Get JNode type
        [[nodiscard]] JNodeType getNodeType() const
        {
            return (m_jNodeData->getNodeType());
        }
        // Get reference to JNodeData
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
    // ==================
    // JNode Object Entry
    // ==================
    struct JNodeObjectEntry
    {
        std::string key;
        JNode::Ptr value;
    };
    // ==========
    // JNode Data
    // ==========
    // Object
    struct JNodeObjectData : JNodeData
    {
        using ObjectEntryList = std::vector<JNodeObjectEntry>;
        // Constructors/Destructors
        JNodeObjectData() : JNodeData(JNodeType::object) {}
        JNodeObjectData(ObjectEntryList &objects) : JNodeData(JNodeType::object), m_jsonObjects(std::move(objects)) {}
        JNodeObjectData(const JNodeObjectData &other) = delete;
        JNodeObjectData &operator=(const JNodeObjectData &other) = delete;
        JNodeObjectData(JNodeObjectData &&other) = default;
        JNodeObjectData &operator=(JNodeObjectData &&other) = default;
        ~JNodeObjectData() = default;
        // Find a given object entry given its key
        [[nodiscard]] auto find(const std::string &key) const
        {
            return (findKey(key, m_jsonObjects));
        }
        // Return true if an object contains a given key
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
        // Return number of entries in an object
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_jsonObjects.size()));
        }
        // Return object entry for a given key
        JNode &operator[](const std::string &key)
        {
            return (*(findKey(key, m_jsonObjects)->value));
        }
        const JNode &operator[](const std::string &key) const
        {
            return (*(findKey(key, m_jsonObjects)->value));
        }
        // Return reference to base of object entries
        ObjectEntryList &objects()
        {
            return (m_jsonObjects);
        }
        [[nodiscard]] const ObjectEntryList &objects() const
        {
            return (m_jsonObjects);
        }

    private:
        static ObjectEntryList::const_iterator findKey(const std::string &key,
                                                       const ObjectEntryList &objects)
        {
            auto entry = std::find_if(objects.begin(), objects.end(), [&key](const JNodeObjectEntry &entry) -> bool
                                      { return (entry.key == key); });
            if (entry == objects.end())
            {
                throw JNode::Error("Invalid key used to access object.");
            }
            return (entry);
        }
        ObjectEntryList m_jsonObjects;
    };
    // Array
    struct JNodeArrayData : JNodeData
    {
        // Constructors/Destructors
        JNodeArrayData() : JNodeData(JNodeType::array) {}
        JNodeArrayData(std::vector<JNode::Ptr> &array) : JNodeData(JNodeType::array),
                                                         m_jsonArray(std::move(array)) {}
        JNodeArrayData(const JNodeArrayData &other) = delete;
        JNodeArrayData &operator=(const JNodeArrayData &other) = delete;
        JNodeArrayData(JNodeArrayData &&other) = default;
        JNodeArrayData &operator=(JNodeArrayData &&other) = default;
        ~JNodeArrayData() = default;
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
        JNode &operator[](std::size_t index)
        {
            if ((index >= 0) && (index < (static_cast<int>(m_jsonArray.size()))))
            {
                return (*m_jsonArray[index]);
            }
            throw JNode::Error("Invalid index used to access array.");
        }
        const JNode &operator[](std::size_t index) const
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
    // Number
    struct JNodeNumberData : JNodeData
    {
        // Constructors/Destructors
        JNodeNumberData() : JNodeData(JNodeType::number) {}
        explicit JNodeNumberData(const std::string &number) : JNodeData(JNodeType::number)
        {
            std::memcpy(&m_jsonNumber[0], number.c_str(), number.size());
        }
        explicit JNodeNumberData(JNodeNumber &number) : JNodeData(JNodeType::number), m_jsonNumber(std::move(number)) {}
        JNodeNumberData(const JNodeNumberData &other) = delete;
        JNodeNumberData &operator=(const JNodeNumberData &other) = delete;
        JNodeNumberData(JNodeNumberData &&other) = default;
        JNodeNumberData &operator=(JNodeNumberData &&other) = default;
        ~JNodeNumberData() = default;
        // Is character a valid numeric character ?
        bool isValidNumeric(char ch) const
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
        // Return reference to number character array
        [[nodiscard]] JNodeNumber &number()
        {
            return (m_jsonNumber);
        }
        [[nodiscard]] const JNodeNumber &number() const
        {
            return (m_jsonNumber);
        }
        // Convert number to string
        [[nodiscard]] std::string toString() const
        {
            return (std::string{m_jsonNumber.begin(), m_jsonNumber.begin() + std::strlen(&m_jsonNumber[0])});
        }

    private:
        JNodeNumber m_jsonNumber{};
    };
    // String
    struct JNodeStringData : JNodeData
    {
        // Constructors/Destructors
        JNodeStringData() : JNodeData(JNodeType::string) {}
        JNodeStringData(const std::string &string) : JNodeData(JNodeType::string),
                                                     m_jsonString(std::move(string)) {}
        JNodeStringData(const JNodeStringData &other) = delete;
        JNodeStringData &operator=(const JNodeStringData &other) = delete;
        JNodeStringData(JNodeStringData &&other) = default;
        JNodeStringData &operator=(JNodeStringData &&other) = default;
        ~JNodeStringData() = default;
        // Return reference to string
        std::string &string()
        {
            return (m_jsonString);
        }
        [[nodiscard]] const std::string &string() const
        {
            return (m_jsonString);
        }
        // Convert string representation to a string
        [[nodiscard]] std::string toString() const
        {
            return (m_jsonString);
        }

    private:
        std::string m_jsonString;
    };
    // Boolean
    struct JNodeBooleanData : JNodeData
    {
        // Constructors/Destructors
        JNodeBooleanData() : JNodeData(JNodeType::boolean) {}
        JNodeBooleanData(bool boolean) : JNodeData(JNodeType::boolean), m_jsonBoolean(boolean) {}
        JNodeBooleanData(const JNodeBooleanData &other) = delete;
        JNodeBooleanData &operator=(const JNodeBooleanData &other) = delete;
        JNodeBooleanData(JNodeBooleanData &&other) = default;
        JNodeBooleanData &operator=(JNodeBooleanData &&other) = default;
        ~JNodeBooleanData() = default;
        // Return boolean value
        [[nodiscard]] bool boolean() const
        {
            return (m_jsonBoolean);
        }
        // Return string representation of boolean value
        [[nodiscard]] std::string toString() const
        {
            return (m_jsonBoolean ? "true" : "false");
        }

    private:
        bool m_jsonBoolean;
    };
    // Null
    struct JNodeNullData : JNodeData
    {
        // Constructors/Destructors
        JNodeNullData() : JNodeData(JNodeType::null) {}
        JNodeNullData(const JNodeNullData &other) = delete;
        JNodeNullData &operator=(const JNodeNullData &other) = delete;
        JNodeNullData(JNodeNullData &&other) = default;
        JNodeNullData &operator=(JNodeNullData &&other) = default;
        ~JNodeNullData() = default;
        // Return null value
        [[nodiscard]] void *null() const
        {
            return (nullptr);
        }
        // Return string representation of null value
        [[nodiscard]] std::string toString() const
        {
            return ("null");
        }
    };
    // Hole
    struct JNodeHoleData : JNodeData
    {
        // Constructors/Destructors
        JNodeHoleData() : JNodeData(JNodeType::hole) {}
        JNodeHoleData(const JNodeHoleData &other) = delete;
        JNodeHoleData &operator=(const JNodeHoleData &other) = delete;
        JNodeHoleData(JNodeHoleData &&other) = default;
        JNodeHoleData &operator=(JNodeHoleData &&other) = default;
        ~JNodeHoleData() = default;
        [[nodiscard]] std::string toString() const
        {
            return ("null");
        }
    };
    // ==============
    // JNode Creation
    // ==============
    inline std::unique_ptr<JNode> makeJNodeObject(JNodeObjectData::ObjectEntryList &objects)
    {
        JNode jNode{std::make_unique<JNodeObjectData>(JNodeObjectData{objects})};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeArray(std::vector<JNode::Ptr> &array)
    {
        JNode jNode{std::make_unique<JNodeArrayData>(JNodeArrayData{array})};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeNumber(JNodeNumber &number)
    {
        JNode jNode{std::make_unique<JNodeNumberData>(JNodeNumberData{number})};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeString(const std::string &string)
    {
        JNode jNode{std::make_unique<JNodeStringData>(JNodeStringData{string})};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeBoolean(bool boolean)
    {
        JNode jNode{std::make_unique<JNodeBooleanData>(JNodeBooleanData{boolean})};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeNull()
    {
        JNode jNode{std::make_unique<JNodeNullData>(JNodeNullData())};
        return (std::make_unique<JNode>(std::move(jNode)));
    }
    inline std::unique_ptr<JNode> makeJNodeHole()
    {
        JNode jNode{std::make_unique<JNodeHoleData>(JNodeHoleData())};
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
    // Object
    inline JNode &JNode::operator[](const std::string &key)
    {
        if (this->getNodeType() == JNodeType::hole)
        {
            this->m_jNodeData = std::make_unique<JNodeObjectData>();
            JNodeDataRef<JNodeObjectData>(*this).objects().emplace_back(JNodeObjectEntry{key, makeJNodeHole()});
            return (*JNodeDataRef<JNodeObjectData>(*this).objects().back().value);
        }
        return (JNodeDataRef<JNodeObjectData>(*this)[key]);
    }
    inline const JNode &JNode::operator[](const std::string &key) const
    {
        return (JNodeDataRef<const JNodeObjectData>(*this)[key]);
    }
    // Array
    inline JNode &JNode::operator[](std::size_t index)
    {
        try
        {
            if (this->getNodeType() == JNodeType::hole)
            {
                this->m_jNodeData = std::make_unique<JNodeArrayData>();
            }
            return (JNodeDataRef<JNodeArrayData>(*this)[index]);
        }
        catch ([[maybe_unused]] const JNode::Error &error)
        {

            JNodeDataRef<JNodeArrayData>(*this).array().resize(index + 1);
            JNodeDataRef<JNodeArrayData>(*this).array()[index] = std::move(makeJNodeHole());
            for (auto &entry : JNodeDataRef<JNodeArrayData>(*this).array()) {
                if (entry.get()==nullptr) {
                    entry = std::move(makeJNodeHole());
                }
            }
            return (JNodeDataRef<JNodeArrayData>(*this)[index]);
        }
    }
    inline const JNode &JNode::operator[](std::size_t index) const
    {
        return (JNodeDataRef<JNodeArrayData>(*this)[index]);
    }
    // ====================
    // Assignment operators
    // ====================
    inline JNode &JNode::operator=(float floatingPoint)
    {
        std::string stringValue{std::to_string(floatingPoint)};
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{stringValue}));
        return (*this);
    }
    inline JNode &JNode::operator=(double floatingPoint)
    {
        std::string stringValue{std::to_string(floatingPoint)};
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{stringValue}));
        return (*this);
    }
    inline JNode &JNode::operator=(long double floatingPoint)
    {
        std::string stringValue{std::to_string(floatingPoint)};
        stringValue.erase(stringValue.find_last_not_of('0') + 1, std::string::npos);
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{stringValue}));
        return (*this);
    }
    inline JNode &JNode::operator=(int integer)
    {
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{std::to_string(integer)}));
        return (*this);
    }
    inline JNode &JNode::operator=(long integer)
    {
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{std::to_string(integer)}));
        return (*this);
    }
    inline JNode &JNode::operator=(long long integer)
    {
        m_jNodeData = std::make_unique<JNodeNumberData>(std::move(JNodeNumberData{std::to_string(integer)}));
        return (*this);
    }
    inline JNode &JNode::operator=(const char *cString)
    {
        m_jNodeData = std::make_unique<JNodeStringData>(std::move(JNodeStringData{cString}));
        return (*this);
    }
    inline JNode &JNode::operator=(const std::string &string)
    {
        m_jNodeData = std::make_unique<JNodeStringData>(std::move(JNodeStringData{string}));
        return (*this);
    }
    inline JNode &JNode::operator=(bool boolean)
    {
        m_jNodeData = std::make_unique<JNodeBooleanData>(std::move(JNodeBooleanData{boolean}));
        return (*this);
    }
    inline JNode &JNode::operator=([[maybe_unused]] std::nullptr_t null)
    {
        m_jNodeData = std::make_unique<JNodeNullData>(std::move(JNodeNullData()));
        return (*this);
    }
} // namespace JSONLib