#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include "JSON_Config.hpp"
#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {

/// @brief Strategy interface for reading and writing file encodings.
class IEncodingHandler
{
public:
  virtual ~IEncodingHandler() = default;

  /// @brief Read encoded text from a file stream into a UTF-8 string.
  /// @param file Input file stream positioned at the content start (or past BOM).
  /// @return UTF-8 std::string.
  virtual std::string read(std::ifstream &file) const = 0;

  /// @brief Write a UTF-8 string to a file stream using target encoding.
  /// @param file Output file stream.
  /// @param utf8Content The UTF-8 string to write.
  virtual void write(std::ofstream &file, const std::string_view &utf8Content) const = 0;
};

} // namespace JSON_Lib
