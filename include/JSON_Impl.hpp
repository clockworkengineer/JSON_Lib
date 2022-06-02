#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <stdexcept>
//
// Source/Destination interfaces
//
#include "ISource.hpp"
#include "IDestination.hpp"
//
// JSON JNodes
//
#include "JSON_Nodes.hpp"
//
// JSON translator
//
#include "JSON_Translator.hpp"
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
        // ============
        // CONSTRUCTORS
        // ============
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        void parse(ISource &source);
        void stringify(IDestination &destination);
        void strip(ISource &source, IDestination &destination);
        JNode &root() { return (*m_jNodeRoot); }
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
        std::string extractString(ISource &source);
        std::string extractValue(ISource &source);
        JNodeObject::Entry extractKeyValuePair(ISource &source);
        JNode::Ptr parseString(ISource &source);
        JNode::Ptr parseNumber(ISource &source);
        JNode::Ptr parseBoolean(ISource &source);
        JNode::Ptr parseNull(ISource &source);
        JNode::Ptr parseObject(ISource &source);
        JNode::Ptr parseArray(ISource &source);
        JNode::Ptr parseJNodes(ISource &source);
        void stringifyJNodes(JNode &jNode, IDestination &destination);
        void stripWhiteSpace(ISource &source, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        JNode::Ptr m_jNodeRoot;
        std::unique_ptr<ITranslator> m_translator;
    };
} // namespace JSONLib
