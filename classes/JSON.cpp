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
// Dependencies:   C17++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "JSONConfig.hpp"
#include "JSON.hpp"
#include "JSONSources.hpp"
#include "JSONDestinations.hpp"
#include "JSONTranslator.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
//
// C++ STL
//
#include <set>
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
    static JSONTranslator defaultTranslator;
    static std::set<char> validNumeric{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', '-', '+', 'E', 'e'};
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
        if (source.current()!='"')
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
        while (source.more() && validNumeric.count(source.current()) > 0)
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
        if (m_workBuffer == "null")
        {
            return (std::make_unique<JNodeNull>());
        }
        throw JSON::SyntaxError();
    }
    /// <summary>
    /// Parse an object from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseObject(ISource &source)
    {
        JNodeObject object;
        do
        {
            source.next();
            source.ignoreWS();
            std::string key = m_jsonTranslator->fromEscapeSequences(extractString(source));
            source.ignoreWS();
            if (source.current() != ':')
            {
                throw JSON::SyntaxError();
            }
            source.next();
            source.ignoreWS();
            object.addEntry(key, parseJNodes(source));
            source.ignoreWS();
        } while (source.current() == ',');
        if (source.current() != '}')
        {
            throw JSON::SyntaxError();
        }
        source.next();
        return (std::make_unique<JNodeObject>(std::move(object)));
    }
    /// <summary>
    /// Parse an array from a JSON source stream.
    /// </summary>
    /// <param name="source">Source for JSON encoded bytes.</param>
    /// <returns></returns>
    JNodePtr JSON::parseArray(ISource &source)
    {
        std::vector<JNodePtr> array;
        do
        {
            source.next();
            source.ignoreWS();
            array.emplace_back(parseJNodes(source));
            source.ignoreWS();
        } while (source.current() == ',');
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
    void JSON::stringifyJNodes(JNode *jNode, IDestination &destination)
    {
        switch (jNode->nodeType)
        {
        case JNodeType::number:
            destination.add(JNodeRef<JNodeNumber>(*jNode).getNumber());
            break;
        case JNodeType::string:
            destination.add("\"" + m_jsonTranslator->toEscapeSequences(JNodeRef<JNodeString>(*jNode).getString()) + "\"");
            break;
        case JNodeType::boolean:
            destination.add(JNodeRef<JNodeBoolean>(*jNode).getBoolean() ? "true" : "false");
            break;
        case JNodeType::null:
            destination.add("null");
            break;
        case JNodeType::object:
        {
            int commaCount = JNodeRef<JNodeObject>(*jNode).size() - 1;
            destination.add("{");
            for (const auto &[key, jNodePtr] : JNodeRef<JNodeObject>(*jNode).getObject())
            {
                destination.add("\"" + m_jsonTranslator->toEscapeSequences(key) + "\"" + ":");
                stringifyJNodes(JNodeRef<JNodeObject>(*jNode).getEntry(key), destination);
                if (commaCount-- > 0)
                {
                    destination.add(",");
                }
            }
            destination.add("}");
            break;
        }
        case JNodeType::array:
        {
            int commaCount = JNodeRef<JNodeArray>(*jNode).size() - 1;
            destination.add("[");
            for (auto &bNodeEntry : JNodeRef<JNodeArray>(*jNode).getArray())
            {
                stringifyJNodes(bNodeEntry.get(), destination);
                if (commaCount-- > 0)
                {
                    destination.add(",");
                }
            }
            destination.add("]");
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
            m_jsonTranslator = &defaultTranslator;
        }
        else
        {
            m_jsonTranslator = translator;
        }
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// Remove all whitespace from a JSON buffer.
    /// </summary>
    /// <param name="jsonBuffer">Buffer of JSON</param>
    /// <returns>Whitespace free JSON.</returns>
    std::string JSON::stripWhiteSpaceBuffer(const std::string &jsonBuffer)
    {
        BufferSource source(jsonBuffer);
        BufferDestination destination;
        while (source.more())
        {
            source.ignoreWS();
            if (source.more())
            {
                if (source.current() == '"')
                {
                    destination.add("\"" + extractString(source) + "\"");
                }
                else
                {
                    destination.add(std::string(1, source.current()));
                    source.next();
                }
            }
        }
        return (destination.getBuffer());
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
        stringifyJNodes(m_jNodeRoot.get(), destination);
    }
    void JSON::stringify(IDestination &&destination)
    {
        stringifyJNodes(m_jNodeRoot.get(), destination);
    }
} // namespace JSONLib
