//
// Class: JSON_Impl_File
//
// Description: JSON class implementation layer to read and write
// JSON files in a number of different formats. Note that these
// methods are all static and do not need a JSON object to invoke.
// For more information on byte marks and their meaning check out link
// https://en.wikipedia.org/wiki/Byte_order_mark.
//
// Dependencies: C++20 - Language standard features used.
//

#include "JSON_Impl.hpp"
#include "JSON_Throw.hpp"

#if !JSON_LIB_NO_STDIO
#include <fstream>
#include <sstream>
#endif

namespace JSON_Lib {

#if !JSON_LIB_NO_STDIO
/// <summary>
/// Write JSON string to a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <param name="jsonString">JSON string</param>
void writeJSONString(std::ofstream &jsonFile, const std::string_view &jsonString) { jsonFile << jsonString; }
/// <summary>
/// Write JSON string to a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <param name="jsonString">JSON string</param>
/// <param name="format">JSON file format</param>
void writeJSONString(std::ofstream &jsonFile, const std::u16string_view &jsonString, const JSON::Format format)
{
  if (format == JSON::Format::utf16BE) {
    jsonFile << static_cast<unsigned char>(0xFE) << static_cast<unsigned char>(0xFF);
    for (const auto ch : jsonString) {
      jsonFile.put(static_cast<char>(ch >> 8));
      jsonFile.put(static_cast<char>(ch));
    }
  } else if (format == JSON::Format::utf16LE) {
    jsonFile << static_cast<char>(0xFF) << static_cast<char>(0xFE);
    for (const auto ch : jsonString) {
      jsonFile.put(static_cast<char>(ch));
      jsonFile.put(static_cast<char>(ch >> 8));
    }
  } else {
    JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) specified in call to writeJSONString()."));
  }
}

/// <summary>
/// Read JSON string from a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <returns>JSON string.</returns>
std::string readJSONString(std::ifstream &jsonFile)
{
  const auto initialPosition = jsonFile.tellg();
  if (initialPosition == -1) {
    return {};
  }
  jsonFile.seekg(0, std::ios_base::end);
  const auto length = jsonFile.tellg();
  jsonFile.seekg(initialPosition, std::ios_base::beg);
  std::string jsonString;
  if (length > 0) {
    jsonString.reserve(static_cast<std::size_t>(length));
  }
  jsonString.assign(std::istreambuf_iterator<char>(jsonFile), {});
  return jsonString;
}
std::u16string readJSONString(std::ifstream &jsonFile, const JSON::Format format)
{
  std::u16string utf16String;
  // Move past byte order mark
  jsonFile.seekg(2);
  if (format == JSON::Format::utf16BE)
    while (true) {
      char16_t ch16 = static_cast<char>(jsonFile.get()) << 8;
      ch16 |= static_cast<char>(jsonFile.get());
      if (jsonFile.eof()) break;
      utf16String.push_back(ch16);
    }
  else if (format == JSON::Format::utf16LE) {
    while (true) {
      char16_t ch16 = static_cast<char>(jsonFile.get());
      ch16 |= static_cast<char>(jsonFile.get()) << 8;
      if (jsonFile.eof()) break;
      utf16String.push_back(ch16);
    }
  } else {
    JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) specified in call to readJSONString()."));
  }
  return utf16String;
}

/// <summary>
/// Return format of JSON file after checking for any byte order marks at
/// the beginning of the JSON file.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON file format.</returns>
JSON::Format JSON_Impl::getFileFormat(const std::string_view &fileName)
{
  std::ifstream jsonFile{ fileName.data(), std::ios_base::binary };
  if (!jsonFile.is_open()) {
    return JSON::Format::utf8;
  }
  const unsigned char first  = static_cast<unsigned char>(jsonFile.get());
  const unsigned char second = static_cast<unsigned char>(jsonFile.get());
  const unsigned char third  = static_cast<unsigned char>(jsonFile.get());
  const unsigned char fourth = static_cast<unsigned char>(jsonFile.get());

  if (first == 0x00u && second == 0x00u && third == 0xFEu && fourth == 0xFFu) { return JSON::Format::utf32BE; }
  if (first == 0xFFu && second == 0xFEu && third == 0x00u && fourth == 0x00u) { return JSON::Format::utf32LE; }
  if (first == 0xEFu && second == 0xBBu) {
    if (third != 0xBFu) {
      JSON_THROW(UnsupportedEncodingError("Invalid UTF-8 Byte Order Mark sequence detected."));
    }
    return JSON::Format::utf8BOM;
  }
  if (first == 0xFEu && second == 0xFFu) { return JSON::Format::utf16BE; }
  if (first == 0xFFu && second == 0xFEu) { return JSON::Format::utf16LE; }
  jsonFile.close();
  return JSON::Format::utf8;
}

/// <summary>
/// Open a JSON file, read its contents into a string buffer and return
/// the buffer. Note any CRLF in the source file are translated to just a
/// LF internally.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <returns>JSON string.</returns>
std::string JSON_Impl::fromFile(const std::string_view &fileName)
{
  constexpr auto kCRLF = "\x0D\x0A";
  // Get file format
  const JSON::Format format = getFileFormat(fileName);
  // Read in JSON
  std::ifstream jsonFile{ fileName.data(), std::ios_base::binary };
  if (!jsonFile.is_open()) {
    return {};
  }
  std::string translated;
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile.seekg(3);// Move past byte order mark
    JSON_LIB_FALLTHROUGH;
  case JSON::Format::utf8:
    translated = readJSONString(jsonFile);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    translated = toUtf8(readJSONString(jsonFile, format));
    break;
  default:
    JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) encountered."));
  }
  jsonFile.close();
  // Translate CRLF -> LF
  size_t pos = translated.find(kCRLF);
  while (pos != std::string::npos) {
    constexpr auto kLF = "\x0A";
    translated.replace(pos, 2, kLF);
    pos = translated.find(kCRLF, pos + 1);
  }
  return translated;
}

/// <summary>
/// Create an JSON file and write JSON string to it.
/// </summary>
/// <param name="fileName">JSON file name</param>
/// <param name="jsonString">JSON string</param>
/// <param name="format">JSON file format</param>
void JSON_Impl::toFile(const std::string_view &fileName, const std::string_view &jsonString, const JSON::Format format)
{
  std::ofstream jsonFile{ fileName.data(), std::ios::binary };
  if (!jsonFile.is_open()) {
    JSON_THROW(Error("File output stream failed to open or cannot be created."));
  }
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile << static_cast<unsigned char>(0xEF) << static_cast<unsigned char>(0xBB)
             << static_cast<unsigned char>(0xBF);
    JSON_LIB_FALLTHROUGH;
  case JSON::Format::utf8:
    writeJSONString(jsonFile, jsonString);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    writeJSONString(jsonFile, toUtf16(jsonString), format);
    break;
  default:
    JSON_THROW(UnsupportedEncodingError("Unsupported JSON file format (Byte Order Mark) specified."));
  }
  jsonFile.close();
}
#endif // !JSON_LIB_NO_STDIO
}// namespace JSON_Lib
