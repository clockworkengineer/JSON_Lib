#pragma once
#include <cwctype>
#include <string>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // =======================================================
    // Interface for reading source stream during JSON parsing
    // =======================================================
    class ISource
    {
    public:
        // =============
        // ISource Error
        // =============
        struct Error : public std::runtime_error
        {
            explicit Error(const std::string &message) : std::runtime_error("ISource Error: " + message)
            {
            }
        };
        // ========================
        // Constructors/destructors
        // ========================
        ISource() = default;
        ISource(const ISource &other) = delete;
        ISource &operator=(const ISource &other) = delete;
        ISource(ISource &&other) = delete;
        ISource &operator=(ISource &&other) = delete;
        virtual ~ISource() = default;
        // =================
        // Current character
        // =================
        [[nodiscard]] virtual char current() const = 0;
        // ======================
        // Move to next character
        // ======================
        virtual void next() = 0;
        // =========================================
        // Are there still more characters to read ?
        // =========================================
        [[nodiscard]] virtual bool more() const = 0;
        // ========================
        // Backup length characters
        // ========================
        virtual void backup(long length) = 0;
        // ===================================
        // Reset to beginning of source stream
        // ===================================
        virtual void reset() = 0;
        // =====================================
        // Is the current character whitespace ?
        // =====================================
        [[nodiscard]] bool isWS() const
        {
            return (std::iswspace(static_cast<wint_t>(current()))) != 0;
        }
        // ==================================
        // Ignore whitespace on source stream
        // ==================================
        void ignoreWS()
        {
            while (more() && isWS())
            {
                next();
            }
        }
        // =================================================================
        // Is current string a match at the current source stream position ?
        // =================================================================
        bool match(const std::string &targetString)
        {
            long index = 0;
            while (more() && current() == targetString[index])
            {
                next();
                if (++index == (long)targetString.length())
                {
                    return (true);
                }
            }
            backup(index);
            return (false);
        }
    };
} // namespace JSONLib