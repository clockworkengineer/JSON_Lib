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

namespace JSON_Lib {

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
    throw Error("Unsupported JSON file format (Byte Order Mark) specified in call to writeJSONString().");
  }
}

/// <summary>
/// Read JSON string from a file stream.
/// </summary>
/// <param name="jsonFile">JSON file stream</param>
/// <returns>JSON string.</returns>
std::string readJSONString(const std::ifstream &jsonFile)
{
  std::ostringstream jsonFileBuffer;
  jsonFileBuffer << jsonFile.rdbuf();
  return jsonFileBuffer.str();
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
    throw Error("Unsupported JSON file format (Byte Order Mark) specified in call to readJSONString().");
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
  uint32_t byteOrderMark = static_cast<unsigned char>(jsonFile.get()) << 24;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get()) << 16;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get()) << 8;
  byteOrderMark |= static_cast<unsigned char>(jsonFile.get());
  if (byteOrderMark == 0x0000FEFF) { return JSON::Format::utf32BE; }
  if (byteOrderMark == 0xFFFE0000) { return JSON::Format::utf32LE; }
  if ((byteOrderMark & 0xFFFFFF00) == 0xEFBBBF00) { return JSON::Format::utf8BOM; }
  if ((byteOrderMark & 0xFFFF0000) == 0xFEFF0000) { return JSON::Format::utf16BE; }
  if ((byteOrderMark & 0xFFFF0000) == 0xFFFE0000) { return JSON::Format::utf16LE; }
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
  const auto kCRLF = "\x0D\x0A";
  // Get file format
  const JSON::Format format = getFileFormat(fileName);
  // Read in JSON
  std::ifstream jsonFile{ fileName.data(), std::ios_base::binary };
  std::string translated;
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile.seekg(3);// Move past byte order mark
  case JSON::Format::utf8:
    translated = readJSONString(jsonFile);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    translated = toUtf8(readJSONString(jsonFile, format));
    break;
  default:
    throw Error("Unsupported JSON file format (Byte Order Mark) encountered.");
  }
  jsonFile.close();
  // Translate CRLF -> LF
  size_t pos = translated.find(kCRLF);
  while (pos != std::string::npos) {
    const auto kLF = "\x0A";
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
  switch (format) {
  case JSON::Format::utf8BOM:
    jsonFile << static_cast<unsigned char>(0xEF) << static_cast<unsigned char>(0xBB)
             << static_cast<unsigned char>(0xBF);
  case JSON::Format::utf8:
    writeJSONString(jsonFile, jsonString);
    break;
  case JSON::Format::utf16BE:
  case JSON::Format::utf16LE:
    writeJSONString(jsonFile, toUtf16(jsonString), format);
    break;
  default:
    throw Error("Unsupported JSON file format (Byte Order Mark) specified.");
  }
  jsonFile.close();
}
}// namespace JSON_Lib
