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
        explicit JSON(ITranslator *translator = nullptr);
        JSON(const JSON &other) = delete;
        JSON &operator=(const JSON &other) = delete;
        JSON(JSON &&other) = delete;
        JSON &operator=(JSON &&other) = delete;
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
