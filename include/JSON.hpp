#pragma once
//
// C++ STL
//
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <list>
#include <memory>
#include <fstream>
#include <codecvt>
#include <locale>
#include <stdexcept>
//
// Source/Destination interfaces
//
#include "ISource.hpp"
#include "IDestination.hpp"
//
// JSON JNodes
//
#include "JNode.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
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
        //
        // JSON translator interface
        //
        class ITranslator
        {
        public:
            virtual ~ITranslator() = default;
            virtual std::string fromEscapeSequences(const std::string &jsonString) = 0;
            virtual std::string toEscapeSequences(const std::string &utf8String) = 0;

        protected:
            [[nodiscard]] static bool isValidSurrogateUpper(char16_t c)
            {
                return ((c >= 0xD800) && (c <= 0xDBFF));
            }
            [[nodiscard]] static bool isValidSurrogateLower(char16_t c)
            {
                return ((c >= 0xDC00) && (c <= 0xDFFF));
            }
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
        std::string stripWhiteSpaceBuffer(const std::string &jsonBuffer);
        void parse(ISource &source);
        void parse(ISource &&source);
        void stringify(IDestination &destination);
        void stringify(IDestination &&destination);
        JNode *getRoot();
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
        std::unique_ptr<JNode> parseJNodes(ISource &source);
        std::unique_ptr<JNode> parseString(ISource &source);
        std::unique_ptr<JNode> parseNumber(ISource &source);
        std::unique_ptr<JNode> parseBoolean(ISource &source);
        std::unique_ptr<JNode> parseNull(ISource &source);
        std::unique_ptr<JNode> parseObject(ISource &source);
        std::unique_ptr<JNode> parseArray(ISource &source);
        void stringifyJNodes(JNode *jNode, IDestination &destination);
        void setTranslator(ITranslator *translator);
        // =================
        // PRIVATE VARIABLES
        // =================
        std::string m_workBuffer;
        std::unique_ptr<JNode> m_jNodeRoot;
        ITranslator *m_jsonTranslator{};
    };
} // namespace JSONLib
