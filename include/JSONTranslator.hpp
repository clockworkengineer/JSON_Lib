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
    class JSONTranslator : public ITranslator
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ============
        // CONSTRUCTORS
        // ============
        JSONTranslator()
        {
            initialiseTranslationMaps();
        }
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        std::string fromEscapeSequences(const std::string &jsonString) override;
        std::string toEscapeSequences(const std::string &utf8String) override;
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
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> m_UTF16;
        std::map<char, std::u16string> m_fromMap;
        std::map<char16_t, std::string> m_toMap;
    };
} // namespace JSONLib
