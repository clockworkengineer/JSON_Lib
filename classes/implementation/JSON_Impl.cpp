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
    /// Extract a string from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <param name="translate">== true and escapes found then they need translating.</param>
    /// <returns>Extracted string</returns>
    std::string JSON_Impl::extractString(ISource &source, bool translate)
    {
        bool translateEscapes = false;
        if (source.current() != '"')
        {
            throw Error("Syntax error detected.");
        }
        source.next();
        std::string stringValue;
        while (source.more() && source.current() != '"')
        {
            if (source.current() == '\\')
            {
                stringValue += source.current();
                source.next();
                translateEscapes = translate;
            }
            stringValue += source.current();
            source.next();
        }
        if (source.current() != '"')
        {
            throw Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        // Need to translate escapes to UTF8
        if (translateEscapes)
        {
            return (m_translator->from(stringValue));
        }
        // None so just pass on
        else
        {
            return (stringValue);
        }
    }
    /// <summary>
    /// Parse a key/value pair from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Object key/value pair.</returns>
    JNodeObject::KeyValuePair JSON_Impl::parseKeyValuePair(ISource &source)
    {
        const std::string keyValue{extractString(source)};
        if (source.current() != ':')
        {
            throw Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (JNodeObject::KeyValuePair{keyValue, parseJNodes(source)});
    }
    /// <summary>
    /// Parse a string from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>String value.</returns>
    JNode::Ptr JSON_Impl::parseString(ISource &source)
    {
        return (std::make_unique<JNodeString>(extractString(source)));
    }
    /// <summary>
    /// Parse a number from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNode::Ptr JSON_Impl::parseNumber(ISource &source)
    {
        JNodeNumber jNodeNumber;
        while (source.more() && JNodeNumber::isValidNumeric(source.current()))
        {
            jNodeNumber.number() += source.current();
            source.next();
        }
        if (!jNodeNumber.isValidNumber())
        {
            throw Error("Syntax error detected.");
        }
        source.ignoreWS();
        return (std::make_unique<JNodeNumber>(std::move(jNodeNumber)));
    }
    /// <summary>
    /// Parse a boolean from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns> Boolean JNode.</returns>
    JNode::Ptr JSON_Impl::parseBoolean(ISource &source)
    {
        if (source.match("true"))
        {
            source.ignoreWS();
            return (std::make_unique<JNodeBoolean>(true));
        }
        if (source.match("false"))
        {
            source.ignoreWS();
            return (std::make_unique<JNodeBoolean>(false));
        }
        throw Error("Syntax error detected.");
    }
    /// <summary>
    /// Parse a null from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Null JNode.</returns>
    JNode::Ptr JSON_Impl::parseNull(ISource &source)
    {
        if (!source.match("null"))
        {
            throw Error("Syntax error detected.");
        }
        source.ignoreWS();
        return (std::make_unique<JNodeNull>());
    }
    /// <summary>
    /// Parse an object from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>JNodeObject key/value pairs.</returns>
    JNode::Ptr JSON_Impl::parseObject(ISource &source)
    {
        JNodeObject jNodeObject;
        source.next();
        source.ignoreWS();
        if (source.current() != '}')
        {
            jNodeObject.objects().emplace_back(parseKeyValuePair(source));
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                jNodeObject.objects().emplace_back(parseKeyValuePair(source));
            }
        }
        if (source.current() != '}')
        {
            throw Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (std::make_unique<JNodeObject>(std::move(jNodeObject)));
    }
    /// <summary>
    /// Parse an array from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>JNodeArray.</returns>
    JNode::Ptr JSON_Impl::parseArray(ISource &source)
    {
        JNodeArray jNodeArray;
        source.next();
        source.ignoreWS();
        if (source.current() != ']')
        {
            jNodeArray.array().emplace_back(parseJNodes(source));
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                jNodeArray.array().emplace_back(parseJNodes(source));
            }
        }
        if (source.current() != ']')
        {
            throw Error("Syntax error detected.");
        }
        source.next();
        source.ignoreWS();
        return (std::make_unique<JNodeArray>(std::move(jNodeArray)));
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
        throw Error("Syntax error detected.");
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
        switch (jNode.getNodeType())
        {
        case JNodeType::number:
            destination.add(JNodeRef<const JNodeNumber>(jNode).number());
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
            for (auto &bNodeEntry : JNodeRef<JNodeArray>(jNode).array())
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
            throw Error("Unknown JNode type encountered during stringification.");
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
                destination.add(source.current());
                if (source.current() == '"')
                {
                    destination.add(extractString(source, false));
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
    JSON_Impl::JSON_Impl()
    {
        m_converter = std::make_unique<JSON_Converter>();
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
        return (std::format("JSONLib Version {}.{}.{}", JSON_VERSION_MAJOR, JSON_VERSION_MINOR, JSON_VERSION_PATCH));
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
    /// Set converter for JSON strings.
    /// </summary>
    /// <param name=conv>Custom JSON string translator.</param>
    void JSON_Impl::converter(IConverter *converter)
    {
        if (converter == nullptr)
        {
            m_converter = std::make_unique<JSON_Converter>();
        }
        else
        {
            m_converter.reset(converter);
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
    JNode::Ptr JSON_Impl::parse(ISource &source)
    {
        return(parseJNodes(source));
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON in destination stream.
    /// </summary>
    /// <param name="jNodeRoot">Root of JNode structure.</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON_Impl::stringify(JNode &jNodeRoot, IDestination &destination)
    {
        stringifyJNodes(jNodeRoot, destination);
    }
} // namespace JSONLib
