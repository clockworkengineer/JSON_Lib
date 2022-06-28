#pragma once
// =======
// C++ STL
// =======
#include <unordered_map>
#include <string>
#include <vector>
#include <array>
#include <stdexcept>
// ===============================
// Translator/Converter interfaces
// ===============================
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
        // ==========
        // JSON Error
        // ==========
        struct Error : public std::runtime_error
        {
            Error(std::string const &message) : std::runtime_error("JSON Translator Error: " + message)
            {
            }
        };
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        JSON_Translator(IConverter &converter);
        JSON_Translator(const JSON_Translator &other) = delete;
        JSON_Translator &operator=(const JSON_Translator &other) = delete;
        JSON_Translator(JSON_Translator &&other) = delete;
        JSON_Translator &operator=(JSON_Translator &&other) = delete;
        ~JSON_Translator() = default;
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
        // ===============
        // PRIVATE METHODS
        // ===============
        // =================
        // PRIVATE VARIABLES
        // =================
        IConverter &m_converter;
        inline static std::unordered_map<char, char16_t> m_from;
        inline static std::unordered_map<char16_t, char> m_to;
    };
} // namespace JSONLib
