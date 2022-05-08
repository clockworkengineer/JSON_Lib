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
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ===================================
    // Forward declarations for interfaces
    // ===================================

    class ISource;
    class IDestination;
    class ITranslator;
    
    // ================
    // CLASS DEFINITION
    // ================
    class JSON
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        //
        // JSON syntax error.
        //
        struct SyntaxError : public std::exception
        {
        public:
            explicit SyntaxError(std::string errorMessage = "JSON syntax error detected.") : errorMessage(std::move(errorMessage)) {}
            [[nodiscard]] const char *what() const noexcept override
            {
                return (errorMessage.c_str());
            }

        private:
            std::string errorMessage;
        };
        // ============
        // CONSTRUCTORS
        // ============
        explicit JSON(ITranslator *translator = nullptr)
        {
            setTranslator(translator);
        }
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        void parse(ISource &source);
        void parse(ISource &&source);
        void stringify(IDestination &destination);
        void stringify(IDestination &&destination);
        void strip(ISource &source, IDestination &destination);
        void strip(ISource &source, IDestination &&destination);
        void strip(ISource &&source, IDestination &destination);
        void strip(ISource &&source, IDestination &&destination);
        JNode &root() { return (*m_jNodeRoot); }
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
        void extractKeyValuePair(ISource &source, std::vector<JNodeObject::Entry> &objects);
        JNodePtr parseString(ISource &source);
        JNodePtr parseNumber(ISource &source);
        JNodePtr parseBoolean(ISource &source);
        JNodePtr parseNull(ISource &source);
        JNodePtr parseObject(ISource &source);
        JNodePtr parseArray(ISource &source);
        JNodePtr parseJNodes(ISource &source);
        void stringifyJNodes(JNode &jNode, IDestination &destination);
        void stripAllWhiteSpace(ISource &source, IDestination &destination);
        void setTranslator(ITranslator *translator);
        // =================
        // PRIVATE VARIABLES
        // =================
        std::string m_workBuffer;
        JNodePtr m_jNodeRoot;
        std::unique_ptr<ITranslator> m_jsonTranslator;
    };
} // namespace JSONLib
