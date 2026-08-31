#pragma once

#include <string>
#include <string_view>
#include "JSON.hpp"

namespace JSON_Lib {

#if !JSON_LIB_NO_STDIO

/// @brief Dedicated service class for handling JSON file reading, writing, and BOM format detection.
class JSON_FileIO
{
public:
  /// @brief Determine file encoding format from leading Byte Order Marks.
  /// @param fileName Path to the target file.
  /// @return Detected JSON::Format.
  static JSON::Format getFileFormat(const std::string_view &fileName);

  /// @brief Read a JSON file and return its decoded UTF-8 string content (with CRLF normalized to LF).
  /// @param fileName Path to the target file.
  /// @return Decoded UTF-8 string content.
  static std::string fromFile(const std::string_view &fileName);

  /// @brief Write a UTF-8 JSON string to a file in the specified format.
  /// @param fileName Path to the target file.
  /// @param jsonString UTF-8 JSON text string.
  /// @param format Target file format.
  static void toFile(const std::string_view &fileName, const std::string_view &jsonString, JSON::Format format);
};

#endif // !JSON_LIB_NO_STDIO

} // namespace JSON_Lib
