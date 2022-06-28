#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ===================================================
    // Forward declarations for interfaces/classes/structs
    // ===================================================
    class JSON_Impl;
    class ISource;
    class IDestination;
    class IConverter;
    class ITranslator;
    struct JNode;
    // ================
    // CLASS DEFINITION
    // ================
    class JSON
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        explicit JSON(ITranslator *translator = nullptr, IConverter *converter = nullptr);
        JSON(const JSON &other) = delete;
        JSON &operator=(const JSON &other) = delete;
        JSON(JSON &&other) = delete;
        JSON &operator=(JSON &&other) = delete;
        ~JSON();
        // ==============
        // PUBLIC METHODS
        // ==============
        std::string version();
        void parse(ISource &source);
        void parse(ISource &&source);
        void stringify(IDestination &destination);
        void stringify(IDestination &&destination);
        void strip(ISource &source, IDestination &destination);
        void strip(ISource &source, IDestination &&destination);
        void strip(ISource &&source, IDestination &destination);
        void strip(ISource &&source, IDestination &&destination);
        JNode &root() { return (*m_jNodeRoot); }
        const JNode &root() const { return (*m_jNodeRoot); }
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
        // =================
        // PRIVATE VARIABLES
        // =================
        // JSON implementation
        static const std::unique_ptr<JSON_Impl> m_implementation;
        // Root of JSON tree node structure.
        std::unique_ptr<JNode> m_jNodeRoot;
    };
} // namespace JSONLib
