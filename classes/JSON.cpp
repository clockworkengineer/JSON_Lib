//
// Class: JSON
//
// Description: Class to perform JSON  stringify/parse to/from a byte
// or file. It is also  possible to customize this with the ISource
// and IDestination interfaces if required. Note: At present it will
// report incorrect JSON syntax but will not be specific about what
// error has occurred; this is reasoned to add too much overhead to
// the process of parsing for the requirements of this library
// (this might change in future versions). For an in-depth description
// of the JSON specification refer to its RFC at web address
// https://tools.ietf.org/html/rfc8259.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSONConfig.hpp"
#include "JSON.hpp"
#include "JSONTranslator.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
//
// C++ STL
//
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
    static bool isValidNumeric(char ch)
    {
        // Includes possible sign, decimal point or exponent
        return ((std::isdigit(ch) != 0) || ch == '.' || ch == '-' || ch == '+' || ch == 'E' || ch == 'e');
    }
    /// <summary>
    /// Extract a string from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>Extracted string</returns>
    std::string JSON::extractString(ISource &source)
    {
        m_workBuffer.clear();
        source.next();
        while (source.more() && source.current() != '"')
        {
            if (source.current() == '\\')
            {
                m_workBuffer += source.current();
                source.next();
            }
            m_workBuffer += source.current();
            source.next();
        }
        if (source.current() != '"')
        {
            throw JSON::SyntaxError();
        }
        source.next();
        return (m_workBuffer);
    }
    /// <summary>
    /// Parse a string from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseString(ISource &source)
    {
        return (std::make_unique<JNodeString>(m_jsonTranslator->fromEscapeSequences(extractString(source))));
    }
    /// <summary>
    /// Parse a number from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseNumber(ISource &source)
    {
        m_workBuffer.clear();
        m_workBuffer += source.current();
        source.next();
        while (source.more() && isValidNumeric(source.current()))
        {
            m_workBuffer += source.current();
            source.next();
        }
        // Throw error if not valid integer or floating point
        char *end = nullptr;
        std::strtoll(m_workBuffer.c_str(), &end, 10);
        if (*end != '\0')
        {
            std::strtod(m_workBuffer.c_str(), &end);
            if (*end != '\0')
            {
                throw JSON::SyntaxError();
            }
        }
        return (std::make_unique<JNodeNumber>(m_workBuffer));
    }
    /// <summary>
    /// Parse a boolean from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseBoolean(ISource &source)
    {
        m_workBuffer.clear();
        m_workBuffer += source.current();
        source.next();
        while (source.more() && (std::isalpha(source.current()) != 0))
        {
            m_workBuffer += source.current();
            source.next();
        }
        if (m_workBuffer == "true")
        {
            return (std::make_unique<JNodeBoolean>(true));
        }
        if (m_workBuffer == "false")
        {
            return (std::make_unique<JNodeBoolean>(false));
        }
        throw JSON::SyntaxError();
    }
    /// <summary>
    /// Parse a null from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseNull(ISource &source)
    {
        m_workBuffer.clear();
        m_workBuffer += source.current();
        source.next();
        while (source.more() && (std::isalpha(source.current()) != 0))
        {
            m_workBuffer += source.current();
            source.next();
        }
        if (m_workBuffer != "null")
        {
            throw JSON::SyntaxError();
        }
        return (std::make_unique<JNodeNull>());
    }
    /// <summary>
    /// Parse an object from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseObject(ISource &source)
    {
        std::vector<JNodeObject::Entry> objects;
        source.next();
        source.ignoreWS();
        if (source.current() != '}')
        {
            std::string key = m_jsonTranslator->fromEscapeSequences(extractString(source));
            source.ignoreWS();
            if (source.current() != ':')
            {
                throw JSON::SyntaxError();
            }
            source.next();
            source.ignoreWS();
            objects.emplace_back(key, parseJNodes(source));
            source.ignoreWS();
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                key = m_jsonTranslator->fromEscapeSequences(extractString(source));
                source.ignoreWS();
                if (source.current() != ':')
                {
                    throw JSON::SyntaxError();
                }
                source.next();
                source.ignoreWS();
                objects.emplace_back(key, parseJNodes(source));
                source.ignoreWS();
            }
        }
        if (source.current() != '}')
        {
            throw JSON::SyntaxError();
        }
        source.next();
        return (std::make_unique<JNodeObject>(objects));
    }
    /// <summary>
    /// Parse an array from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseArray(ISource &source)
    {
        std::vector<JNodePtr> array;
        source.next();
        source.ignoreWS();
        if (source.current() != ']')
        {
            array.emplace_back(parseJNodes(source));
            source.ignoreWS();
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                array.emplace_back(parseJNodes(source));
                source.ignoreWS();
            }
        }
        if (source.current() != ']')
        {
            throw JSON::SyntaxError();
        }
        source.next();
        return (std::make_unique<JNodeArray>(array));
    }
    /// <summary>
    /// Recursively parse JSON source stream producing a JNode structure
    /// representation  of it.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseJNodes(ISource &source)
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
        throw JSON::SyntaxError();
    }
    /// <summary>
    /// Recursively traverse JNode structure encoding it into JSON on
    /// the destination stream passed in.
    /// </summary>
    /// <param name=jNode>JNode structure to be traversed</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON::stringifyJNodes(JNode &jNode, IDestination &destination)
    {
        switch (jNode.nodeType)
        {
        case JNodeType::number:
            destination.add(JNodeRef<JNodeNumber>(jNode).number());
            break;
        case JNodeType::string:
            destination.add('"');
            destination.add(m_jsonTranslator->toEscapeSequences(JNodeRef<JNodeString>(jNode).string()));
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
            for (const auto &[key, jNodePtr] : JNodeRef<JNodeObject>(jNode).objects())
            {
                destination.add('"');
                destination.add(m_jsonTranslator->toEscapeSequences(key));
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
            throw std::runtime_error("Unknown JNode type encountered during stringification.");
        }
    }
    /// <summary>
    /// Set translator for JSON strings.
    /// </summary>
    /// <param name=translator>Custom JSON string translator.</param>
    void JSON::setTranslator(ITranslator *translator)
    {
        if (translator == nullptr)
        {
            m_jsonTranslator = std::make_unique<JSONTranslator>();
        }
        else
        {
            m_jsonTranslator.reset(translator);
        }
    }
    /// <summary>
    /// Strip all whitespace from a JSON source.
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destinaton">Destination for stripped JSON</param>
    void JSON::stripAllWhiteSpace(ISource &source, IDestination &destination)
    {
        while (source.more())
        {
            source.ignoreWS();
            if (source.more())
            {
                destination.add(source.current());
                if (source.current() == '"')
                {
                    destination.add(extractString(source));
                    destination.add('"');
                }
                else
                {
                    source.next();
                }
            }
        }
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// Strip all whitespace from a JSON source .
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destinaton">Destination for stripped JSON</param>
    void JSON::strip(ISource &source, IDestination &destination)
    {
        stripAllWhiteSpace(source, destination);
    }
    void JSON::strip(ISource &source, IDestination &&destination)
    {
        stripAllWhiteSpace(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &destination)
    {
        stripAllWhiteSpace(source, destination);
    }
    void JSON::strip(ISource &&source, IDestination &&destination)
    {
        stripAllWhiteSpace(source, destination);
    }
    /// <summary>
    /// Create JNode structure by recursively parsing JSON on the source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.
    void JSON::parse(ISource &source)
    {
        m_jNodeRoot = parseJNodes(source);
    }
    void JSON::parse(ISource &&source)
    {
        m_jNodeRoot = parseJNodes(source);
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON in destination stream.
    /// </summary>
    /// <param name="jNodeRoot">Root of JNode structure.</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSON::stringify(IDestination &destination)
    {
        if (m_jNodeRoot.get() == nullptr)
        {
            throw std::runtime_error("No JSON to stringify.");
        }
        stringifyJNodes(JNodeRef<JNode>(*m_jNodeRoot), destination);
    }
    void JSON::stringify(IDestination &&destination)
    {
        if (m_jNodeRoot.get() == nullptr)
        {
            throw std::runtime_error("No JSON to stringify.");
        }
        stringifyJNodes(JNodeRef<JNode>(*m_jNodeRoot), destination);
    }
} // namespace JSONLib
