#pragma once
//
// C++ STL
//
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <array>
#include <iterator>
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
        JSON_Translator(IConverter &converter);
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        std::string from(const ISource::String &jsonString) override;
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
        IConverter &m_converter;
        std::unordered_map<char, ISource::Char> m_from;
        std::unordered_map<ISource::Char, char> m_to;
    };
} // namespace JSONLib
