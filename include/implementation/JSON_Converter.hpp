#pragma once
// ==========================
// C++ STL/ platform specific
// ==========================
#if defined(_WIN64)
#include "Windows.h"
#else
#include <codecvt>
#include <locale>
#endif
// ===================
// Converter interface
// ===================
#include "IConverter.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class JSON_Converter : public IConverter
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        // ==============
        // PUBLIC METHODS
        // ==============
        virtual std::u16string to_utf16(const std::string &utf8) override;
        virtual std::string to_utf8(const std::u16string &utf16) override;
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
#if !defined(_WIN64)
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> m_UTF16;
#endif
    };
} // namespace JSONLib
