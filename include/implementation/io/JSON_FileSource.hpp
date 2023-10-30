#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "ISource.hpp"

namespace JSON_Lib {

class FileSource : public ISource
{
public:
  explicit FileSource(const std::string &filename) : filename(filename)
  {
    source.open(filename.c_str(), std::ios_base::binary);
    if (!source.is_open()) { throw Error("File input stream failed to open or does not exist."); }
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  virtual ~FileSource() = default;

  virtual char current() const override { return (static_cast<char>(source.peek())); }
  virtual void next() override
  {
    if (!more()) { throw Error("Tried to read past end of file."); }
    source.get();
    if (current() == kCarriageReturn) {
      source.get();
      if (current() != kLineFeed) { source.unget(); }
    }
    m_column++;
    if (current() == kLineFeed) {
      m_lineNo++;
      m_column = 1;
    }
  }
  virtual bool more() const override { return (source.peek() != EOF); }
  virtual void reset() override
  {
    m_lineNo = 1;
    m_column = 1;
    source.clear();
    source.seekg(0, std::ios_base::beg);
  }
  virtual std::size_t position() const override
  {
    if (more()) { return (static_cast<std::size_t>(source.tellg())); }
    return (std::filesystem::file_size(filename));
  }

private:
  virtual void backup(unsigned long length) override
  {
    source.clear();
    source.seekg(-static_cast<long>(length), std::ios_base::cur);
  }

  mutable std::ifstream source;
  std::string filename;
};
}// namespace JSON_Lib
