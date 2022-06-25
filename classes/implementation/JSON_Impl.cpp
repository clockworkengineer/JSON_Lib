//
// Class: JSON_Impl
//
// Description: JSON class implementation layer.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSON_Impl.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ===========================
    // PRIVATE TYPES AND CONSTANTS
    // ===========================
    // ==========================
    // PUBLIC TYPES AND CONSTANTS
    // ==========================
    // ========================
    // PRIVATE STATIC VARIABLES
    // ========================
    // =======================
    // PUBLIC STATIC VARIABLES
    // =======================
    // ===============
    // PRIVATE METHODS
    // ===============
    /// <summary>
    /// Is a valid numerical character.
    /// </summary>
    /// <param name="ch">Numeric character.</param>
    /// <returns>true if a character use in a number</returns>
    bool JSON_Impl::isValidNumeric(ISource::Char c)
    {
        // Includes possible sign, decimal point or exponent
        return ((std::isdigit(c) != 0) || c == '.' || c == '-' || c == '+' || c == 'E' || c == 'e');
    }
    /// <summary>
    /// Extract a string from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Extracted string</returns>
    ISource::String JSON_Impl::extractString(ISource &source)
    {
        if (source.current() != '"')
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        source.next();
        ISource::String value;
        while (source.more() && source.current() != '"')
        {
            if (source.current() == '\\')
            {
                value += source.current();
                source.next();
            }
            value += source.current();
            source.next();
        }
        if (source.current() != '"')
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (value);
    }
    /// <summary>
    /// Parse alphabetic value from source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>String alphabetic value "true"/"false"/"null".</returns>
    ISource::String JSON_Impl::parseValue(ISource &source)
    {
        ISource::String value;
        while (source.more() && (std::isalpha(source.current()) != 0))
        {
            value += source.current();
            source.next();
        }
        source.ignoreWS();
        return (value);
    }
    /// <summary>
    /// Parse a key/value pair from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Object key/value pair.</returns>
    JNodeObject::KeyValuePair JSON_Impl::parseKeyValuePair(ISource &source)
    {
        const std::string key{m_translator->from(extractString(source))};
        if (source.current() != ':')
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (JNodeObject::KeyValuePair{key, parseJNodes(source)});
    }
    /// <summary>
    /// Parse a string from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>String value.</returns>
    JNode::Ptr JSON_Impl::parseString(ISource &source)
    {
        return (std::make_unique<JNodeString>(m_translator->from(extractString(source))));
    }
    /// <summary>
    /// Parse a number from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNode::Ptr JSON_Impl::parseNumber(ISource &source)
    {
        ISource::String value;
        while (source.more() && isValidNumeric(source.current()))
        {
            value += source.current();
            source.next();
        }
        // Throw error if not valid integer or floating point
        char *end = nullptr;
        std::string number = m_converter->to_utf8(value);
        std::strtoll(number.c_str(), &end, 10);
        if (*end != '\0')
        {
            std::strtod(number.c_str(), &end);
            if (*end != '\0')
            {
                throw JSONLib::Error("Syntax error detected.");
            }
        }
        source.ignoreWS();
        return (std::make_unique<JNodeNumber>(number));
    }
    /// <summary>
    /// Parse a boolean from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns> Boolean JNode.</returns>
    JNode::Ptr JSON_Impl::parseBoolean(ISource &source)
    {
        const std::string value{m_converter->to_utf8(parseValue(source))};
        if (value == "true")
        {
            return (std::make_unique<JNodeBoolean>(true));
        }
        if (value == "false")
        {
            return (std::make_unique<JNodeBoolean>(false));
        }
        throw JSONLib::Error("Syntax error detected.");
    }
    /// <summary>
    /// Parse a null from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Null JNode.</returns>
    JNode::Ptr JSON_Impl::parseNull(ISource &source)
    {
        if (parseValue(source) != u"null")
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        return (std::make_unique<JNodeNull>());
    }
    /// <summary>
    /// Parse an object from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>JNodeObject key/value pairs.</returns>
    JNode::Ptr JSON_Impl::parseObject(ISource &source)
    {
        std::vector<JNodeObject::KeyValuePair> objects;
        source.next();
        source.ignoreWS();
        if (source.current() != '}')
        {
            objects.emplace_back(parseKeyValuePair(source));
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                objects.emplace_back(parseKeyValuePair(source));
            }
        }
        if (source.current() != '}')
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (std::make_unique<JNodeObject>(objects));
    }
    /// <summary>
    /// Parse an array from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>JNodeArray.</returns>
    JNode::Ptr JSON_Impl::parseArray(ISource &source)
    {
        std::vector<JNode::Ptr> array;
        source.next();
        source.ignoreWS();
        if (source.current() != ']')
        {
            array.emplace_back(parseJNodes(source));
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                array.emplace_back(parseJNodes(source));
            }
        }
        if (source.current() != ']')
        {
            throw JSONLib::Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (std::make_unique<JNodeArray>(array));
    }
    /// <summary>
    /// Recursively parse JSON source stream producing a JNode structure
    /// representation  of it.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNode::Ptr JSON_Impl::parseJNodes(ISource &source)
    {
        source.ignoreWS();
        switch (source.current())
        {
        case '"':
            return (parseString(source));
        case 't':
        case 'f':
            return (parseBoolean(source));
        case 'n':
            return (parseNull(source));
        case '{':
            return (parseObject(source));
        case '[':
            return (parseArray(source));
        case '-':
        case '+':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (parseNumber(source));
        }
        throw JSONLib::Error("Syntax error detected.");
    }
    /// <summary>
    /// Recursively traverse JNode structure encoding it into JSON on
    /// the destination stream passed in.
    /// </summary>
    /// <param name=jNode>JNode structure to be traversed</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON_Impl::stringifyJNodes(const JNode &jNode, IDestination &destination)
    {
        switch (jNode.nodeType)
        {
        case JNodeType::number:
            destination.add(JNodeRef<JNodeNumber>(jNode).number());
            break;
        case JNodeType::string:
            destination.add('"');
            destination.add(m_translator->to(JNodeRef<JNodeString>(jNode).string()));
            destination.add('"');
            break;
        case JNodeType::boolean:
            destination.add(JNodeRef<JNodeBoolean>(jNode).boolean() ? "true" : "false");
            break;
        case JNodeType::null:
            destination.add("null");
            break;
        case JNodeType::object:
        {
            int commaCount = JNodeRef<JNodeObject>(jNode).size() - 1;
            destination.add('{');
            for (auto &[key, jNodePtr] : JNodeRef<JNodeObject>(jNode).objects())
            {
                destination.add('"');
                destination.add(m_translator->to(key));
                destination.add("\":");
                stringifyJNodes(JNodeRef<JNodeObject>(jNode)[key], destination);
                if (commaCount-- > 0)
                {
                    destination.add(',');
                }
            }
            destination.add('}');
            break;
        }
        case JNodeType::array:
        {
            int commaCount = JNodeRef<JNodeArray>(jNode).size() - 1;
            destination.add('[');
            for (const auto &bNodeEntry : JNodeRef<JNodeArray>(jNode).array())
            {
                stringifyJNodes(JNodeRef<JNode>(*bNodeEntry), destination);
                if (commaCount-- > 0)
                {
                    destination.add(',');
                }
            }
            destination.add(']');
            break;
        }
        default:
            throw std::runtime_error("Unknown JNode type encountered during stringification.");
        }
    }
    /// <summary>
    /// Strip all whitespace from a JSON source.
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destination">Destination for stripped JSON</param>
    void JSON_Impl::stripWhiteSpace(ISource &source, IDestination &destination)
    {
        while (source.more())
        {
            if (!source.isWS())
            {
                destination.add(m_converter->to_utf8(ISource::String{source.current()}));
                if (source.current() == '"')
                {
                    destination.add(m_converter->to_utf8(extractString(source)));
                    destination.add('"');
                    continue;
                }
            }
            source.next();
        }
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// JSON_Impl constructor.
    /// </summary>
    JSON_Impl::JSON_Impl() : m_converter(std::make_unique<JSON_Converter>())
    {
    }
    /// <summary>
    /// JSON_Impl destructor.
    /// </summary>
    JSON_Impl::~JSON_Impl()
    {
    }
    /// <summary>
    ///  Get JSONLib version.
    /// </summary>
    std::string JSON_Impl::version()
    {
        return (std::format("JSONLib Version {}.{}", JSON_VERSION_MAJOR, JSON_VERSION_MINOR));
    }
    /// <summary>
    /// Set translator for JSON strings.
    /// </summary>
    /// <param name=translator>Custom JSON string translator.</param>
    void JSON_Impl::translator(ITranslator *translator)
    {
        if (translator == nullptr)
        {
            m_translator = std::make_unique<JSON_Translator>(*m_converter);
        }
        else
        {
            m_translator.reset(translator);
        }
    }
    /// <summary>
    /// Strip all whitespace from a JSON source.
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destination">Destination for stripped JSON</param>
    void JSON_Impl::strip(ISource &source, IDestination &destination)
    {
        stripWhiteSpace(source, destination);
    }
    /// <summary>
    /// Create JNode structure by recursively parsing JSON on the source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.
    void JSON_Impl::parse(ISource &source)
    {
        m_jNodeRoot = parseJNodes(source);
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON in destination stream.
    /// </summary>
    /// <param name="jNodeRoot">Root of JNode structure.</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON_Impl::stringify(IDestination &destination)
    {
        if (m_jNodeRoot.get() == nullptr)
        {
            throw std::runtime_error("No JSON to stringify.");
        }
        stringifyJNodes(JNodeRef<JNode>(*m_jNodeRoot), destination);
    }
} // namespace JSONLib
