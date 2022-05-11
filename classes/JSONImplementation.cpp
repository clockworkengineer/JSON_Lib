//
// Class: JSONImplementation
//
// Description: JSON class implementation
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSON.hpp"
#include "JSONImplementation.hpp"
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
    std::string JSONImplementation::extractString(ISource &source)
    {
        std::string value;
        source.next();
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
            throw JSONLib::SyntaxError();
        }
        source.next();
        return (value);
    }
    /// <summary>
    /// Extract alphabetic value from source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns>String alphabetic value true/false/null</returns>
    std::string JSONImplementation::extractValue(ISource &source)
    {
        std::string value;
        value += source.current();
        source.next();
        while (source.more() && (std::isalpha(source.current()) != 0))
        {
            value += source.current();
            source.next();
        }
        return(value);
    }
    /// <summary>
    /// Extract a key/value pair from a JSON encoded source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <param name="objects">Vector of object key/values.</param>
    void JSONImplementation::extractKeyValuePair(ISource &source, std::vector<JNodeObject::Entry> &objects)
    {
        std::string key = m_translator->fromEscapeSequences(extractString(source));
        source.ignoreWS();
        if (source.current() != ':')
        {
            throw JSONLib::SyntaxError();
        }
        source.next();
        source.ignoreWS();
        objects.emplace_back(key, parseJNodes(source));
        source.ignoreWS();
    }
    /// <summary>
    /// Parse a string from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseString(ISource &source)
    {
        return (std::make_unique<JNodeString>(m_translator->fromEscapeSequences(extractString(source))));
    }
    /// <summary>
    /// Parse a number from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseNumber(ISource &source)
    {
        std::string value;
        value += source.current();
        source.next();
        while (source.more() && isValidNumeric(source.current()))
        {
            value += source.current();
            source.next();
        }
        // Throw error if not valid integer or floating point
        char *end = nullptr;
        std::strtoll(value.c_str(), &end, 10);
        if (*end != '\0')
        {
            std::strtod(value.c_str(), &end);
            if (*end != '\0')
            {
                throw JSONLib::SyntaxError();
            }
        }
        return (std::make_unique<JNodeNumber>(value));
    }
    /// <summary>
    /// Parse a boolean from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseBoolean(ISource &source)
    {
        std::string value = extractValue(source);
        if (value == "true")
        {
            return (std::make_unique<JNodeBoolean>(true));
        }
        if (value == "false")
        {
            return (std::make_unique<JNodeBoolean>(false));
        }
        throw JSONLib::SyntaxError();
    }
    /// <summary>
    /// Parse a null from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseNull(ISource &source)
    {
        if (extractValue(source) != "null")
        {
            throw JSONLib::SyntaxError();
        }
        return (std::make_unique<JNodeNull>());
    }
    /// <summary>
    /// Parse an object from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseObject(ISource &source)
    {
        std::vector<JNodeObject::Entry> objects;
        source.next();
        source.ignoreWS();
        if (source.current() != '}')
        {
            extractKeyValuePair(source, objects);
            while (source.current() == ',')
            {
                source.next();
                source.ignoreWS();
                extractKeyValuePair(source, objects);
            }
        }
        if (source.current() != '}')
        {
            throw JSONLib::SyntaxError();
        }
        source.next();
        return (std::make_unique<JNodeObject>(objects));
    }
    /// <summary>
    /// Parse an array from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSONImplementation::parseArray(ISource &source)
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
            throw JSONLib::SyntaxError();
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
    JNodePtr JSONImplementation::parseJNodes(ISource &source)
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
        throw JSONLib::SyntaxError();
    }
    /// <summary>
    /// Recursively traverse JNode structure encoding it into JSON on
    /// the destination stream passed in.
    /// </summary>
    /// <param name=jNode>JNode structure to be traversed</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSONImplementation::stringifyJNodes(JNode &jNode, IDestination &destination)
    {
        switch (jNode.nodeType)
        {
        case JNodeType::number:
            destination.add(JNodeRef<JNodeNumber>(jNode).number());
            break;
        case JNodeType::string:
            destination.add('"');
            destination.add(m_translator->toEscapeSequences(JNodeRef<JNodeString>(jNode).string()));
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
                destination.add(m_translator->toEscapeSequences(key));
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
    /// Strip all whitespace from a JSON source.
    /// </summary>
    /// <param name="source">Source of JSON</param>
    /// <param name="destinaton">Destination for stripped JSON</param>
    void JSONImplementation::stripWhiteSpace(ISource &source, IDestination &destination)
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
    /// Set translator for JSON strings.
    /// </summary>
    /// <param name=translator>Custom JSON string translator.</param>
    void JSONImplementation::translator(ITranslator *translator)
    {
        if (translator == nullptr)
        {
            m_translator = std::make_unique<JSONTranslator>();
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
    /// <param name="destinaton">Destination for stripped JSON</param>
    void JSONImplementation::strip(ISource &source, IDestination &destination)
    {
        stripWhiteSpace(source, destination);
    }
    /// <summary>
    /// Create JNode structure by recursively parsing JSON on the source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.
    void JSONImplementation::parse(ISource &source)
    {
        m_jNodeRoot = parseJNodes(source);
    }
    /// <summary>
    /// Recursively parse JNode structure and building its JSON in destination stream.
    /// </summary>
    /// <param name="jNodeRoot">Root of JNode structure.</param>
    /// <param name=destination>destination stream for stringified JSON</param>
    /// <returns></returns>
    void JSONImplementation::stringify(IDestination &destination)
    {
        if (m_jNodeRoot.get() == nullptr)
        {
            throw std::runtime_error("No JSON to stringify.");
        }
        stringifyJNodes(JNodeRef<JNode>(*m_jNodeRoot), destination);
    }
} // namespace JSONLib
