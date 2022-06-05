#pragma once
//
// C++ STL
//
#include <map>
#include <string>
//
// Translator interface
//
#include "ITranslator.hpp"
//
// UTF8/UTF16 converter.
//
#include "JSON_Converter.hpp"
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
        JSON_Translator(IConverter *converter=nullptr)
        {
            initialiseTranslationMaps();
            if (converter==nullptr) {
                m_converter = std::make_unique<JSON_Converter>();
            } else
            {
                m_converter.reset(converter);
            }
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
        std::unique_ptr<IConverter> m_converter;
        std::map<char, std::u16string> m_from;
        std::map<char16_t, std::string> m_to;
    };
} // namespace JSONLib
