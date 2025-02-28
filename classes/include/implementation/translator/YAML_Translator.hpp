

#pragma once

#include "ITranslator.hpp"

namespace JSON_Lib {

    class YAML_Translator final : public ITranslator {
    public:
        // ==============
        // Translator Error
        // ==============
        struct Error final : std::runtime_error {
            explicit Error(const std::string &message)
                : std::runtime_error("Translator Error: " + message) {}
        };
        // ===============================================
        // Translate string to escape sequences ("\uxxxx")
        // ===============================================
        [[nodiscard]] std::string from(const std::string &escapedString) const override {
            return escapedString;
        }
        [[nodiscard]] std::string to(const std::string &escapedString) const override {
            std::string translated;
            for (const char ch : escapedString) {
                if (!isprint(ch)) {
                    const auto digits = "0123456789ABCDEF";
                    translated += "\\u00";
                    translated += digits[ch >> 4 & 0x0f];
                    translated += digits[ch&0x0f];
                } else {
                    if (ch=='\"') {
                        translated += "\\";
                    }
                    translated += ch;
                }
            }
            return translated;
        }
    };
} // namespace JSON_Lib
