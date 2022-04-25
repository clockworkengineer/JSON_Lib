#pragma once
//
// C++ STL
//
#include <fstream>
#include <string>
namespace JSONLib
{
    //
    // Destination classes for stringification.
    //
    class BufferDestination : public IDestination
    {
    public:
        BufferDestination()
        = default;
        void add(const std::string &bytes) override
        {
            for (auto b : bytes)
            {
                m_stringifyBuffer.push_back(b);
            }
        }
        [[nodiscard]] std::string getBuffer() const
        {
            return (m_stringifyBuffer);
        }
    private:
        std::string m_stringifyBuffer;
    };
    class FileDestination : public IDestination
    {
    public:
        explicit FileDestination(const std::string &desinationFileName)
        {
            m_destination.open(desinationFileName.c_str(), std::ios_base::binary);
            if (!m_destination.is_open())
            {
                throw std::runtime_error("JSON file output stream failed to open or could not be created.");
            }
        }
        void add(const std::string &bytes) override
        {
            m_destination.write(bytes.c_str(), bytes.length());
            m_destination.flush();
        }
    private:
        std::ofstream m_destination;
    };
} // namespace JSONLib