#pragma once
//
// C++ STL
//
#include <map>
#include <string>
#include <codecvt>
#include <locale>
//
// Translator interface
//
#include "ITranslator.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class JSON_Translator : public ITranslator
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ============
        // CONSTRUCTORS
        // ============
        JSON_Translator()
        {
            initialiseTranslationMaps();
        }
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        std::string from(const std::string &jsonString) override;
        std::string to(const std::string &utf8String) override;
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
        void initialiseTranslationMaps();
        // =================
        // PRIVATE VARIABLES
        // =================
        std::map<char, std::u16string> m_from;
        std::map<char16_t, std::string> m_to;
    };
} // namespace JSONLib
