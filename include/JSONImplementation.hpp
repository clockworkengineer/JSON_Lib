#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <stdexcept>
//
// JSON JNodes
//
#include "JNode.hpp"
//
// JSON translator
//
#include "JSONTranslator.hpp"
//
// Source/Destination interfaces
//
#include "ISource.hpp"
#include "IDestination.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class JSONImplementation
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
        void extractKeyValuePair(ISource &source, std::vector<JNodeObject::Entry> &objects);
        JNodePtr parseString(ISource &source);
        JNodePtr parseNumber(ISource &source);
        JNodePtr parseBoolean(ISource &source);
        JNodePtr parseNull(ISource &source);
        JNodePtr parseObject(ISource &source);
        JNodePtr parseArray(ISource &source);
        JNodePtr parseJNodes(ISource &source);
        void stringifyJNodes(JNode &jNode, IDestination &destination);
        void stripWhiteSpace(ISource &source, IDestination &destination);

        // =================
        // PRIVATE VARIABLES
        // =================
        JNodePtr m_jNodeRoot;
        std::unique_ptr<ITranslator> m_translator;
    };
} // namespace JSONLib
