#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace JSONLib
{
    // ==========
    // JSON Error
    // ==========
    struct Error : public std::exception
    {
    public:
        explicit Error(std::string message = "") : errorMessage(std::string("JSON Error: ") + message) {}
        [[nodiscard]] const char *what() const noexcept override
        {
            return (errorMessage.c_str());
        }
    private:
        const std::string errorMessage;
    };
} // namespace JSONLib
