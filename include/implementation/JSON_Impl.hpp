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
        JNode::Ptr parse(ISource &source);
        void stringify(JNode &jNodeRoot, IDestination &destination);
        void strip(ISource &source, IDestination &destination);
        static void translator(ITranslator *translator);
        static void converter(IConverter *converter);
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
        static std::string extractString(ISource &source, bool translate = true);
        static JNodeObject::KeyValuePair parseKeyValuePair(ISource &source);
        static JNode::Ptr parseString(ISource &source);
        static JNode::Ptr parseNumber(ISource &source);
        static JNode::Ptr parseBoolean(ISource &source);
        static JNode::Ptr parseNull(ISource &source);
        static JNode::Ptr parseObject(ISource &source);
        static JNode::Ptr parseArray(ISource &source);
        static JNode::Ptr parseJNodes(ISource &source);
        static void stringifyJNodes(const JNode &jNode, IDestination &destination);
        static void stripWhiteSpace(ISource &source, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        inline static std::unique_ptr<ITranslator> m_translator;
        inline static std::unique_ptr<IConverter> m_converter;
    };
} // namespace JSONLib
