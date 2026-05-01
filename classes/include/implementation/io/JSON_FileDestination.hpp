#pragma once
#include "JSON_Throw.hpp"

#include <fstream>
#include <string>
#include <string_view>

#include "JSON_DestinationBase.hpp"

#if JSON_LIB_NO_STDIO
#error "FileDestination is disabled when JSON_LIB_NO_STDIO is enabled."
#endif

namespace JSON_Lib {

class FileDestination final : public DestinationBase
{
public:
  explicit FileDestination(const std::string_view &filename) : filename(filename)
  {
    destination.open(filename.data(), std::ios_base::binary | std::ios_base::trunc);
  }
  FileDestination() = delete;

  void add(const char ch) override
  {
    if (ch == '\n') {
      destination.write("\r\n", 2);
      fileSize += 2;
    } else {
      destination.put(ch);
      fileSize++;
    }
    trackLast(ch);
  }
  void add(const std::string &bytes) override { add(std::string_view{bytes}); }
  void add(const std::string_view &bytes) override
  {
    for (const auto ch : bytes) { add(ch); }
    destination.flush();
  }
  void add(const char *bytes) override { add(std::string_view{bytes}); }
  void clear() override
  {
    if (destination.is_open()) { destination.close(); }
    destination.open(filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!destination.is_open()) { JSON_THROW(Error("File output stream failed to open or could not be created.")); }
    fileSize = 0;
    trackLast(0);
  }
  std::string getFileName() { return filename; }
  void close() { destination.close(); }
  std::size_t size() const { return fileSize; }

private:
  std::ofstream destination;
  std::string filename;
  std::size_t fileSize{};
};
}// namespace JSON_Lib
