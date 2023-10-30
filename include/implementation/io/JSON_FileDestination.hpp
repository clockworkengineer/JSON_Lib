#pragma once

#include <fstream>
#include <string>

#include "IDestination.hpp"

namespace JSON_Lib {

class FileDestination : public IDestination
{
public:
  explicit FileDestination(const std::string &filename) : filename(filename)
  {
    destination.open(filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
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
      destination.write("\r\n", 2);
    } else {
      destination.put(ch);
    }
  }
  virtual void add(const std::string &bytes) override
  {
    for (auto ch : bytes) { add(ch); }
  }
  virtual void clear() override
  {
    if (destination.is_open()) { destination.close(); }
    destination.open(filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!destination.is_open()) { throw Error("File output stream failed to open or could not be created."); }
  }
  void close() { destination.flush(); }

private:
  std::ofstream destination;
  std::string filename;
};
}// namespace JSON_Lib
