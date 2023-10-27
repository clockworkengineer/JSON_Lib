#pragma once

#include <fstream>
#include <string>

#include "IDestination.hpp"

namespace JSON_Lib {

class FileDestination : public IDestination
{
public:
  explicit FileDestination(const std::string &filename) : m_filename(filename)
  {
    m_destination.open(filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  virtual ~FileDestination() = default;

  virtual void add(const char ch) override
  {
    if (ch == '\n') {
      m_destination.write("\r\n", 2);
    } else {
      m_destination.put(ch);
    }
  }
  virtual void add(const std::string &bytes) override
  {
    for (auto ch : bytes) { add(ch); }
  }
  virtual void clear() override
  {
    if (m_destination.is_open()) { m_destination.close(); }
    m_destination.open(m_filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!m_destination.is_open()) { throw Error("File output stream failed to open or could not be created."); }
  }
  void close() { m_destination.flush(); }

private:
  std::ofstream m_destination;
  std::string m_filename;
};
}// namespace JSON_Lib
