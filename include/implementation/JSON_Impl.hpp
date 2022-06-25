#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
#include <stdexcept>
#include <format>
// =============================
// Source/Destination interfaces
// =============================
#include "ISource.hpp"
#include "IDestination.hpp"
// ====
// JSON
// ====
#include "JSON_Config.hpp"
#include "JSON_Types.hpp"
#include "JSON_Translator.hpp"
#include "JSON_Converter.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class JSON_Impl
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        JSON_Impl();
        JSON_Impl(const JSON_Impl &other) = delete;
        JSON_Impl &operator=(const JSON_Impl &other) = delete;
        JSON_Impl(JSON_Impl &&other) = delete;
        JSON_Impl &operator=(JSON_Impl &&other) = delete;
        ~JSON_Impl();
        // ==============
        // PUBLIC METHODS
        // ==============
           std::string version();
        void parse(ISource &source);
        void stringify(IDestination &destination);
        void strip(ISource &source, IDestination &destination);
        JNode &root() { return (*m_jNodeRoot); }
        const JNode &root() const { return (*m_jNodeRoot); }
        void translator(ITranslator *translator);
        // ================
        // PUBLIC VARIABLES
        // ================
    private:
        // ===========================
        // PRIVATE TYPES AND CONSTANTS
        // ===========================
        // ===========================================
        // DISABLED CONSTRUCTORS/DESTRUCTORS/OPERATORS
        // ===========================================
        // ===============
        // PRIVATE METHODS
        // ===============
        bool isValidNumeric(ISource::Char c);
        ISource::String extractString(ISource &source);
        JNodeObject::KeyValuePair parseKeyValuePair(ISource &source);
        JNode::Ptr parseString(ISource &source);
        JNode::Ptr parseNumber(ISource &source);
        JNode::Ptr parseBoolean(ISource &source);
        JNode::Ptr parseNull(ISource &source);
        JNode::Ptr parseObject(ISource &source);
        JNode::Ptr parseArray(ISource &source);
        JNode::Ptr parseJNodes(ISource &source);
        void stringifyJNodes(const JNode &jNode, IDestination &destination);
        void stripWhiteSpace(ISource &source, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        JNode::Ptr m_jNodeRoot;
        std::unique_ptr<ITranslator> m_translator;
        std::unique_ptr<IConverter> m_converter;
    };
} // namespace JSONLib
