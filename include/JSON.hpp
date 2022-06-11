#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
//
// JSON version
//
#include "JSON_Config.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // =================
    // JSON syntax error
    // =================
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
    // ===================================================
    // Forward declarations for interfaces/classes/structs
    // ===================================================
    class ISource;
    class IDestination;
    class ITranslator;
    class JSON_Impl;
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
        // ============
        // CONSTRUCTORS
        // ============
        explicit JSON(ITranslator *translator = nullptr);
        // ==========
        // DESTRUCTOR
        // ==========
        ~JSON();
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
        JNode &root();
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
        const std::unique_ptr<JSON_Impl> m_implementation;
    };
} // namespace JSONLib
