#pragma once
//
// C++ STL
//
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <array>
//
// Translator/Converter interfaces
//
#include "ITranslator.hpp"
#include "IConverter.hpp"
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
        JSON_Translator(IConverter *converter = nullptr);
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
        // =================
        // PRIVATE VARIABLES
        // =================
        std::unique_ptr<IConverter> m_converter;
        std::unordered_map<char, char16_t> m_from;
        std::unordered_map<char16_t, char> m_to;
    };
} // namespace JSONLib
